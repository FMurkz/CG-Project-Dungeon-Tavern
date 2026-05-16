#include "modules/Starter.hpp"
#include <chrono>
// What data each vertex has. For a triangle, just position.
struct Vertex {
    glm::vec3 pos;
};

// Per-object data sent to the shader
struct UniformBufferObject {
    alignas(16) glm::mat4 mvpMat;
};

// Per-frame global data (lighting, viewer position)
struct GlobalUniformBufferObject {
    alignas(16) glm::vec3 lightDir;
    alignas(16) glm::vec4 lightColor;
    alignas(16) glm::vec3 eyePos;
};

class DungeonTavern : public BaseProject {
protected:
    // Vertex layout + descriptor set layout
    VertexDescriptor VD;
    DescriptorSetLayout DSL;          // for per-object uniform buffer
    DescriptorSetLayout DSLglobal;    // for the global uniform buffer

    // Render pass + pipeline
    RenderPass RP;
    Pipeline P;

    // Geometry (the triangle for now)
    Model M_triangle;

    // Descriptor sets (one per object + one global)
    DescriptorSet DS_triangle;
    DescriptorSet DS_global;

    // Aspect ratio for projection matrix
    float Ar;
    // Camera state
    float camYaw = 0.0f;     // horizontal angle, radians
    float camPitch = 0.0f;   // vertical angle, radians
    float camDist = 3.0f;    // distance from origin
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
    void localInit() {
        // 1. Declare what the global descriptor set looks like (1 uniform buffer)
        DSLglobal.init(this, {
            {0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS,
             sizeof(GlobalUniformBufferObject), 1}
        });

        // 2. Declare what the per-object descriptor set looks like (1 uniform buffer)
        DSL.init(this, {
            {0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT,
             sizeof(UniformBufferObject), 1}
        });

        // 3. Declare the vertex layout: one attribute (position) at location 0
        VD.init(this, {
            {0, sizeof(Vertex), VK_VERTEX_INPUT_RATE_VERTEX}
        }, {
            {0, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, pos),
             sizeof(glm::vec3), POSITION}
        });

        // 4. Initialize the render pass with default attachments
        RP.init(this);
        RP.properties[0].clearValue = {0.05f, 0.05f, 0.1f, 1.0f};  // dark blue clear

        // 5. Build the pipeline (shaders + vertex layout + descriptor layouts)
        P.init(this, &VD,
               "shaders/shader.vert.spv",
               "shaders/shader.frag.spv",
               {&DSLglobal, &DSL});
        P.setCullMode(VK_CULL_MODE_NONE);

        // 8 corners of a unit cube centered at origin
        std::vector<Vertex> cubeVerts = {
            {{-0.5f, -0.5f, -0.5f}},  // 0: left-bottom-back
            {{ 0.5f, -0.5f, -0.5f}},  // 1: right-bottom-back
            {{ 0.5f,  0.5f, -0.5f}},  // 2: right-top-back
            {{-0.5f,  0.5f, -0.5f}},  // 3: left-top-back
            {{-0.5f, -0.5f,  0.5f}},  // 4: left-bottom-front
            {{ 0.5f, -0.5f,  0.5f}},  // 5: right-bottom-front
            {{ 0.5f,  0.5f,  0.5f}},  // 6: right-top-front
            {{-0.5f,  0.5f,  0.5f}}   // 7: left-top-front
        };

        M_triangle.vertices.resize(cubeVerts.size() * sizeof(Vertex));
        memcpy(M_triangle.vertices.data(), cubeVerts.data(),
               cubeVerts.size() * sizeof(Vertex));

        // 12 triangles (2 per face × 6 faces), 36 indices total
        M_triangle.indices = {
            // back face
            0, 1, 2,   2, 3, 0,
            // front face
            4, 6, 5,   6, 4, 7,
            // left face
            0, 3, 7,   7, 4, 0,
            // right face
            1, 5, 6,   6, 2, 1,
            // bottom face
            0, 4, 5,   5, 1, 0,
            // top face
            3, 2, 6,   6, 7, 3
        };
        M_triangle.initMesh(this, &VD);

        // 7. Set the descriptor pool sizes
        DPSZs.uniformBlocksInPool = 2;   // global + per-object
        DPSZs.texturesInPool = 0;        // none yet
        DPSZs.setsInPool = 2;            // global + per-object

        // 8. Set initial aspect ratio
        Ar = (float)windowWidth / (float)windowHeight;

        // Register the command buffer callback with the framework
        submitCommandBuffer("main", 0, populateCommandBufferAccess, this);
    }
    void pipelinesAndDescriptorSetsInit() {
        // Create the render pass + pipeline against the current swapchain
        RP.create();
        P.create(&RP);

        // Allocate the actual descriptor sets
        DS_global.init(this, &DSLglobal, {});       // no textures, just the UBO
        DS_triangle.init(this, &DSL, {});           // same
    }

    void pipelinesAndDescriptorSetsCleanup() {
        P.cleanup();
        RP.cleanup();

        DS_global.cleanup();
        DS_triangle.cleanup();
    }

    void localCleanup() {
        M_triangle.cleanup();
        P.destroy();
        RP.destroy();
        DSL.cleanup();
        DSLglobal.cleanup();
    }

    static void populateCommandBufferAccess(VkCommandBuffer commandBuffer, int currentImage, void *Params) {
        DungeonTavern *T = (DungeonTavern *)Params;
        T->populateCommandBuffer(commandBuffer, currentImage);
    }
    void populateCommandBuffer(VkCommandBuffer commandBuffer, int currentImage) {
        // Begin the render pass (clears the framebuffer to dark blue)
        RP.begin(commandBuffer, currentImage);

        // Use our pipeline
        P.bind(commandBuffer);

        // Bind the global descriptor set at set 0
        DS_global.bind(commandBuffer, P, 0, currentImage);

        // Bind the per-object descriptor set at set 1
        DS_triangle.bind(commandBuffer, P, 1, currentImage);

        // Bind the triangle's vertex + index buffers
        M_triangle.bind(commandBuffer);

        // Draw 3 indices = 1 triangle
        vkCmdDrawIndexed(commandBuffer,
                         static_cast<uint32_t>(M_triangle.indices.size()),
                         1, 0, 0, 0);

        // End the render pass
        RP.end(commandBuffer);
    }
    void updateUniformBuffer(uint32_t currentImage) {
        // Exit on ESC
        if (glfwGetKey(window, GLFW_KEY_ESCAPE)) {
            glfwSetWindowShouldClose(window, GL_TRUE);
        }

        // Update aspect ratio in case window was resized
        Ar = (float)windowWidth / (float)windowHeight;

        // Camera: positioned at (0, 0, 2) looking at origin
        // Pull camera back so the cube fits
        // Compute deltaT for frame-rate-independent movement
        static auto lastTime = std::chrono::high_resolution_clock::now();
        auto currentTime = std::chrono::high_resolution_clock::now();
        float deltaT = std::chrono::duration<float>(currentTime - lastTime).count();
        lastTime = currentTime;

        // Rotate camera with WASD
        const float ROT_SPEED = glm::radians(90.0f);  // 90 degrees per second
        if (glfwGetKey(window, GLFW_KEY_A)) camYaw   -= ROT_SPEED * deltaT;  // orbit left
        if (glfwGetKey(window, GLFW_KEY_D)) camYaw   += ROT_SPEED * deltaT;  // orbit right
        if (glfwGetKey(window, GLFW_KEY_W)) camPitch += ROT_SPEED * deltaT;  // tilt up
        if (glfwGetKey(window, GLFW_KEY_S)) camPitch -= ROT_SPEED * deltaT;  // tilt down

        // Clamp pitch so you don't flip upside down
        const float pitchLimit = glm::radians(85.0f);
        if (camPitch >  pitchLimit) camPitch =  pitchLimit;
        if (camPitch < -pitchLimit) camPitch = -pitchLimit;

        // Compute camera position on a sphere around the origin
        glm::vec3 cameraPos = glm::vec3(
            camDist * cos(camPitch) * sin(camYaw),
            camDist * sin(camPitch),
            camDist * cos(camPitch) * cos(camYaw)
        );

        glm::mat4 view = glm::lookAt(
            cameraPos,
            glm::vec3(0.0f),           // looking at origin
            glm::vec3(0.0f, 1.0f, 0.0f) // up is +Y
        );

        glm::mat4 proj = glm::perspective(
            glm::radians(45.0f), Ar, 0.1f, 100.0f
        );
        proj[1][1] *= -1;

        // Cube stays at origin — no animation
        glm::mat4 model = glm::mat4(1.0f);
        // Build per-object UBO
        UniformBufferObject ubo{};
        ubo.mvpMat = proj * view * model;
        DS_triangle.map(currentImage, &ubo, 0);

        // Build global UBO (we declared it, the shader doesn't use it yet,
        // but we have to populate it so Vulkan doesn't read garbage)
        GlobalUniformBufferObject gubo{};
        gubo.lightDir = glm::vec3(0.0f, -1.0f, 0.0f);
        gubo.lightColor = glm::vec4(1.0f);
        gubo.eyePos = cameraPos;
        DS_global.map(currentImage, &gubo, 0);
    }
};

int main() {
    DungeonTavern app;
    try {
        app.run();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}