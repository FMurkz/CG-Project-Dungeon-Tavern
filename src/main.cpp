#include "modules/Starter.hpp"
#include "SceneObjects.hpp"
#include <chrono>

struct Vertex {
    glm::vec3 pos;
    glm::vec3 normal;
};

struct GlobalUniformBufferObject {
    alignas(16) glm::vec3 lightDir;
    alignas(16) glm::vec4 lightColor;
    alignas(16) glm::vec3 eyePos;
};

class DungeonTavern : public BaseProject {
protected:
    // Layouts and Pipelines
    VertexDescriptor VD;
    DescriptorSetLayout DSL_Object;
    DescriptorSetLayout DSL_Global;
    RenderPass RP;
    Pipeline P;

    // All scene objects
    SceneObjects scene;

    // Global descriptor set
    DescriptorSet DS_Global;

    // Camera State
    float Ar = 0.0f;
    glm::vec3 cameraPos = glm::vec3(0.0f, 1.5f, -3.5f);
    float camYaw = 0.0f;
    float camPitch = 0.0f;

    void setWindowParameters() {
        windowWidth = 800;
        windowHeight = 600;
        windowTitle = "Dungeon Tavern - Scene";
        windowResizable = GLFW_TRUE;
        initialBackgroundColor = {0.05f, 0.05f, 0.1f, 1.0f};
    }

    void onWindowResize(int w, int h) {
        RP.width = w;
        RP.height = h;
        Ar = (float)w / (float)h;
    }

    void localInit() {
        DSL_Global.init(this, {
            {0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS,
             sizeof(GlobalUniformBufferObject), 1}
        });

        DSL_Object.init(this, {
            {0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT,
             sizeof(UniformBufferObject), 1}
        });

        VD.init(this,
            {{0, sizeof(Vertex), VK_VERTEX_INPUT_RATE_VERTEX}},
            {
                {0, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, pos),    sizeof(glm::vec3), POSITION},
                {0, 1, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, normal), sizeof(glm::vec3), NORMAL}
            }
        );

        RP.init(this);
        RP.properties[0].clearValue = {0.05f, 0.05f, 0.1f, 1.0f};

        P.init(this, &VD, "shaders/shader.vert.spv", "shaders/shader.frag.spv",
               {&DSL_Global, &DSL_Object});
        P.setCullMode(VK_CULL_MODE_BACK_BIT);

        scene.loadAll(this, &VD);

        DPSZs.uniformBlocksInPool = scene.count() + 1;
        DPSZs.setsInPool          = scene.count() + 1;
        DPSZs.texturesInPool      = 0;

        Ar = (float)windowWidth / (float)windowHeight;
        submitCommandBuffer("main", 0, populateCommandBufferAccess, this);
    }

    void pipelinesAndDescriptorSetsInit() {
        RP.create();
        P.create(&RP);
        DS_Global.init(this, &DSL_Global, {});
        scene.initDescriptorSets(this, &DSL_Object);
    }

    void pipelinesAndDescriptorSetsCleanup() {
        P.cleanup();
        RP.cleanup();
        DS_Global.cleanup();
        scene.cleanupDescriptorSets();
    }

    void localCleanup() {
        scene.cleanupAll();
        P.destroy();
        RP.destroy();
        DSL_Object.cleanup();
        DSL_Global.cleanup();
    }

    static void populateCommandBufferAccess(VkCommandBuffer cb, int img, void *p) {
        ((DungeonTavern *)p)->populateCommandBuffer(cb, img);
    }

    void populateCommandBuffer(VkCommandBuffer commandBuffer, int currentImage) {
        RP.begin(commandBuffer, currentImage);
        P.bind(commandBuffer);

        DS_Global.bind(commandBuffer, P, 0, currentImage);
        scene.drawAll(commandBuffer, P, currentImage);

        RP.end(commandBuffer);
    }

    void updateUniformBuffer(uint32_t currentImage) {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE)) glfwSetWindowShouldClose(window, GL_TRUE);

        // Timer
        static auto lastTime = std::chrono::high_resolution_clock::now();
        auto currentTime = std::chrono::high_resolution_clock::now();
        float deltaT = std::chrono::duration<float>(currentTime - lastTime).count();
        lastTime = currentTime;

        // Input
        constexpr float ROT_SPEED = glm::radians(90.0f);
        if (glfwGetKey(window, GLFW_KEY_D))    camYaw   -= ROT_SPEED * deltaT;
        if (glfwGetKey(window, GLFW_KEY_A))    camYaw   += ROT_SPEED * deltaT;
        if (glfwGetKey(window, GLFW_KEY_UP))   camPitch += ROT_SPEED * deltaT;
        if (glfwGetKey(window, GLFW_KEY_DOWN)) camPitch -= ROT_SPEED * deltaT;
        camPitch = glm::clamp(camPitch, glm::radians(-85.0f), glm::radians(85.0f));

        const glm::vec3 forward = glm::vec3(
            cos(camPitch) * sin(camYaw),
            sin(camPitch),
            cos(camPitch) * cos(camYaw)
        );

        glm::vec3 walkDir = glm::normalize(glm::vec3(forward.x, 0.0f, forward.z));
        constexpr float MOVE_SPEED = 3.0f;
        if (glfwGetKey(window, GLFW_KEY_W)) cameraPos += walkDir * MOVE_SPEED * deltaT;
        if (glfwGetKey(window, GLFW_KEY_S)) cameraPos -= walkDir * MOVE_SPEED * deltaT;

        // Matrices
        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + forward, glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 proj = glm::perspective(glm::radians(45.0f), Ar, 0.1f, 100.0f);
        proj[1][1] *= -1;

        // Update all object UBOs (transforms live in SceneObjects)
        scene.updateUBOs(currentImage, proj, view);

        // Global UBO (lighting + camera)
        GlobalUniformBufferObject gubo{};
        gubo.lightDir   = glm::normalize(glm::vec3(-0.5f, -1.0f, -0.3f));
        gubo.lightColor = glm::vec4(1.0f);
        gubo.eyePos     = cameraPos;
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