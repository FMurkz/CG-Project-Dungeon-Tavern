#include "modules/Starter.hpp"
#include "sceneObjects.hpp"
#include "NPCInteraction.hpp"
#include "DialogBox.hpp"
#include "CollisionSystem.hpp"
#include "miniaudio.h"
#include "TorchInteraction.hpp"
#include <chrono>
#include <vector>

struct Vertex {
    glm::vec3 pos;
    glm::vec3 normal;
    glm::vec2 uv;
};

//Use alignas to enforce std140 layout
//This has to match the PointLight struct in shader.frag
struct PointLight {
    alignas(16) glm::vec3 position;
    alignas(16) glm::vec3 color;
};

//This has to match the GlobalUniformBufferObject struct in shader.frag
struct GlobalUniformBufferObject {
    alignas(16) glm::vec3 lightDir;
    alignas(16) glm::vec4 lightColor;
    alignas(16) glm::vec3 eyePos;
    alignas(16) PointLight torchLight[4];
    alignas(4)  float time;
};

//Inherits BaseProject, BaseProject owns run() (the main loop). it calls the
//lifecycle methods below, they are overwritten here.
class DungeonTavern : public BaseProject {
protected:
    //For music
    ma_engine audioEngine;

    //Vertex layouts (3D / 2D UI)
    VertexDescriptor VD;
    VertexDescriptor VD_UI;

    //Descriptor set layouts, resources each shader expects
    DescriptorSetLayout DSL_Object;
    DescriptorSetLayout DSL_Global;
    DescriptorSetLayout DSL_UI;

    //Render pass + pipelines (3D scene / 2D UI overlay)
    RenderPass RP;
    Pipeline P;
    Pipeline P_UI;

    // Scene + game systems
    SceneObjects scene;

    // Collision handling
    CollisionSystem collisionSystem;

    // Torch interactions
    TorchInteraction torchInteraction;

    // Dialog UI overlay
    DialogBox dialogBox;

    // Global descriptor set
    DescriptorSet DS_Global;

    // Camera State
    float Ar = 0.0f;
    glm::vec3 cameraPos = glm::vec3(0.0f, 1.8f, 9.0f);
    float camYaw = glm::radians(180.0f);
    float camPitch = 0.0f;
    float elapsedTime = 0.0f;

    //NPC interaction state
    std::vector<NPCInteraction> npcs;
    std::vector<DialogBox>      npcDialogs;
    int activeNpcIndex      = -1;   // index of the currently open dialog, -1 = none
    int previousActiveIndex = -1;

    void setWindowParameters() {
        windowWidth = 800;
        windowHeight = 600;
        windowTitle = "Dungeon Tavern";
        windowResizable = GLFW_TRUE;
        initialBackgroundColor = {0.05f, 0.05f, 0.1f, 1.0f};
    }

    void onWindowResize(int w, int h) {
        RP.width = w;
        RP.height = h;
        Ar = (float)w / (float)h;
    }

    //Called once in run()
    void localInit() {
        // Descriptor set layouts (resource schemas)
        DSL_Global.init(this, {
            {0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS,
             sizeof(GlobalUniformBufferObject), 1}
        });

        DSL_Object.init(this, {
            {0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT,
             sizeof(UniformBufferObject), 1},
            {1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT,
         0, 1}
        });

        DSL_UI.init(this, {
        {0, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
         VK_SHADER_STAGE_FRAGMENT_BIT,
         0,
         1}
        });

        //Vertex layouts
        VD.init(this,
            {{0, sizeof(Vertex), VK_VERTEX_INPUT_RATE_VERTEX}},
            {
                {0, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, pos),    sizeof(glm::vec3), POSITION},
                {0, 1, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, normal), sizeof(glm::vec3), NORMAL},
                {0, 2, VK_FORMAT_R32G32_SFLOAT,    offsetof(Vertex, uv),     sizeof(glm::vec2), UV}
            }
        );

        VD_UI.init(this,
    {{0, sizeof(DialogVertex), VK_VERTEX_INPUT_RATE_VERTEX}},
    {
        {0, 0, VK_FORMAT_R32G32_SFLOAT,
         offsetof(DialogVertex, pos),
         sizeof(glm::vec2),
         POS2D},

        {0, 1, VK_FORMAT_R32G32_SFLOAT,
         offsetof(DialogVertex, uv),
         sizeof(glm::vec2),
         UV}
            }
        );

        //Render pass + pipelines
        RP.init(this);
        RP.properties[0].clearValue = {0.05f, 0.05f, 0.1f, 1.0f};

        P.init(this, &VD, "shaders/shader.vert.spv", "shaders/shader.frag.spv",
               {&DSL_Global, &DSL_Object});
        P.setCullMode(VK_CULL_MODE_NONE);

        P_UI.init(this,
          &VD_UI,
          "shaders/dialogbox.vert.spv",
          "shaders/dialogbox.frag.spv",
          {&DSL_UI});

        P_UI.setCullMode(VK_CULL_MODE_NONE);
        P_UI.setTransparency(true); // dialog boxes blend over the scene

        //Load models/textures + register collision
        scene.loadAll(this, &VD);
        scene.registerColliders(collisionSystem);

        //-------------------INTERACTIONS---------------------
        // Torch toggling (3.0f = interaction range)
        torchInteraction.init(scene.getTorchPositions(), 3.0f);
        // NPC definitions: position, interaction range, dialog image.
        std::vector<NPCInteractionDef> npcDefs = {
            { glm::vec3( 2.0f, 0.0f,  2.0f), 3.0f, "assets/ui/innkeeper_dialog.png" },
            { glm::vec3(-6.3f, 0.0f,  7.3f), 3.0f, "assets/ui/orc_dialog.png"       },
        };
        // Build interaction + dialog objects from each definition
        for (auto& def : npcDefs) {
            npcs.emplace_back(def);
            npcDialogs.emplace_back();
            npcDialogs.back().init(this, &VD_UI, def.dialogTexturePath);
        }
        //--------------------------------------------------------

        // Size the descriptor pool; must cover every texture/UBO/set used
        int sceneTextures = scene.count();
        int uiTextures    = static_cast<int>(npcDialogs.size());
        DPSZs.texturesInPool      = sceneTextures + uiTextures;
        DPSZs.uniformBlocksInPool = scene.count() + 1;           // objects + global
        DPSZs.setsInPool          = scene.count() + 1 + uiTextures; // objects + global + UI dialogs

        // Finalize: aspect ratio, first command buffer, start music
        Ar = (float)windowWidth / (float)windowHeight;
        submitCommandBuffer("main", 0, populateCommandBufferAccess, this);

        //Music config
        ma_engine_init(NULL, &audioEngine);
        ma_engine_play_sound(&audioEngine, "assets/audio/tavern_ambience.mp3", NULL);
    }
    //Runs once at startup and on every window resize
    //Rebuilds everything that depends on the swapchain (on resize everything needs to be rebuilt): Render pass, pipelines, descriptor sets
    void pipelinesAndDescriptorSetsInit() {
        RP.create();

        P.create(&RP);
        P_UI.create(&RP);

        DS_Global.init(this, &DSL_Global, {});
        scene.initDescriptorSets(this, &DSL_Object);

        for (auto& dlg : npcDialogs)
            dlg.initDescriptorSet(this, &DSL_UI);
    }

    void pipelinesAndDescriptorSetsCleanup() {
        P.cleanup();
        P_UI.cleanup();
        RP.cleanup();
        DS_Global.cleanup();
        scene.cleanupDescriptorSets();

        for (auto& dlg : npcDialogs)
            dlg.cleanupDescriptorSet();
    }

    //Runs on shutdown
    void localCleanup() {
        scene.cleanupAll();

        for (auto& dlg : npcDialogs)
            dlg.cleanup();
        RP.destroy();
        P.destroy();
        P_UI.destroy();

        DSL_Object.cleanup();
        DSL_Global.cleanup();
        DSL_UI.cleanup();

        VD.cleanup();
        VD_UI.cleanup();

        //Music
        ma_engine_uninit(&audioEngine);
    }
    //Bridges the framework's C-style callback to our C++ method.
    static void populateCommandBufferAccess(VkCommandBuffer cb, int img, void *p) {
        ((DungeonTavern *)p)->populateCommandBuffer(cb, img);
    }

    //runs only when the command list needs re-recording
    //When what is being drawn changes: for this case it is whether a dialog box is showing.
    void populateCommandBuffer(VkCommandBuffer commandBuffer, int currentImage) {
        RP.begin(commandBuffer, currentImage);

        // Draw 3D scene
        P.bind(commandBuffer);

        DS_Global.bind(commandBuffer, P, 0, currentImage);
        scene.drawAll(commandBuffer, P, currentImage);

        //Draw active dialog box on top, if one is open
        if (activeNpcIndex >= 0) {
            P_UI.bind(commandBuffer);
            npcDialogs[activeNpcIndex].draw(commandBuffer, P_UI, currentImage);
        }

        RP.end(commandBuffer);
    }

    //Called every frame
    void updateUniformBuffer(uint32_t currentImage) {
        // Close program if ESC is pressed
        if (glfwGetKey(window, GLFW_KEY_ESCAPE)) glfwSetWindowShouldClose(window, GL_TRUE);

        //Timing variables for deltaT
        static auto lastTime = std::chrono::high_resolution_clock::now();
        auto currentTime = std::chrono::high_resolution_clock::now();
        float deltaT = std::chrono::duration<float>(currentTime - lastTime).count();
        lastTime = currentTime;

        //Camera rotation
        constexpr float ROT_SPEED = glm::radians(90.0f);
        if (glfwGetKey(window, GLFW_KEY_D))    camYaw   -= ROT_SPEED * deltaT;
        if (glfwGetKey(window, GLFW_KEY_A))    camYaw   += ROT_SPEED * deltaT;
        if (glfwGetKey(window, GLFW_KEY_UP))   camPitch += ROT_SPEED * deltaT;
        if (glfwGetKey(window, GLFW_KEY_DOWN)) camPitch -= ROT_SPEED * deltaT;
        camPitch = glm::clamp(camPitch, glm::radians(-85.0f), glm::radians(85.0f));

        //Look direction (derived from camera rotation)
        const glm::vec3 forward = glm::vec3(
            cos(camPitch) * sin(camYaw),
            sin(camPitch),
            cos(camPitch) * cos(camYaw)
        );

        glm::vec3 walkDir = glm::normalize(glm::vec3(forward.x, 0.0f, forward.z)); // Y=0.0 so you don't start flying when looking up
        constexpr float MOVE_SPEED = 1.8f;

        //Movement forward/backward + collision
        glm::vec3 movementDelta = glm::vec3(0.0f);

        if (glfwGetKey(window, GLFW_KEY_W)) {
            movementDelta += walkDir * MOVE_SPEED * deltaT;
        }

        if (glfwGetKey(window, GLFW_KEY_S)) {
            movementDelta -= walkDir * MOVE_SPEED * deltaT;
        }
        cameraPos = collisionSystem.movePlayer(cameraPos, movementDelta);

        //Interactions: NPC update, torch toggle, command-buffer re-record
        int newActive = -1;
        for (int i = 0; i < static_cast<int>(npcs.size()); i++) {
            if (newActive == -1)
                npcs[i].update(window, cameraPos);
            if (npcs[i].hasInteracted())
                newActive = i;
        }
        activeNpcIndex = newActive;
        if (glfwGetKey(window, GLFW_KEY_W)) {
            movementDelta += walkDir * MOVE_SPEED * deltaT;
        }

        if (glfwGetKey(window, GLFW_KEY_S)) {
            movementDelta -= walkDir * MOVE_SPEED * deltaT;
        }

        cameraPos = collisionSystem.movePlayer(cameraPos, movementDelta);

        // Toggle the nearest torch on/off with F
        torchInteraction.update(window, cameraPos);

        // NPC interaction
        if (activeNpcIndex != previousActiveIndex) {
            previousActiveIndex = activeNpcIndex;
            submitCommandBuffer("main", 0, populateCommandBufferAccess, this);
        }
        //Matrices (view + projection)
        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + forward, glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 proj = glm::perspective(glm::radians(45.0f), Ar, 0.1f, 100.0f);
        proj[1][1] *= -1; //Vulkan's clip-space Y is inverted vs OpenGL

        //Update per object UBO
        scene.updateUBOs(
            currentImage,
            proj,
            view,
            activeNpcIndex,
            cameraPos
        );
        //Time
        elapsedTime += deltaT;

        //Global UBO: build the lighting/camera struct that shader.frag reads as 'gubo'
        GlobalUniformBufferObject gubo{};
        gubo.lightDir   = glm::normalize(glm::vec3(-0.5f, -1.0f, -0.3f));
        gubo.lightColor = glm::vec4(1.0f);
        gubo.eyePos     = cameraPos;
        gubo.time       = elapsedTime;

        //Torch positions (four corners of the room)
        constexpr float ROOM_HALF_T = 10.0f;
        constexpr float LIGHT_OFFSET = 3.0f;
        gubo.torchLight[0].position = glm::vec3(-ROOM_HALF_T + 0.6f, LIGHT_OFFSET,  4.0f);
        gubo.torchLight[1].position = glm::vec3(-ROOM_HALF_T + 0.6f, LIGHT_OFFSET, -4.0f);
        gubo.torchLight[2].position = glm::vec3( ROOM_HALF_T - 0.6f, LIGHT_OFFSET,  4.0f);
        gubo.torchLight[3].position = glm::vec3( ROOM_HALF_T - 0.6f, LIGHT_OFFSET, -4.0f);

        // Warm orange torch color, multiplied by intensity
        glm::vec3 torchColor = glm::vec3(1.0f, 0.55f, 0.15f) * 4.0f;
        for (int i = 0; i < 4; ++i) {
            if (torchInteraction.isLit(i)) {
                gubo.torchLight[i].color = torchColor;
            } else {
                gubo.torchLight[i].color = glm::vec3(0.0f);
            }
        }

        //Upload the global UBO to the GPU
        DS_Global.map(currentImage, &gubo, 0);
    }
};

int main() {
    try {
        DungeonTavern app;
        app.run();
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    return 0;
}