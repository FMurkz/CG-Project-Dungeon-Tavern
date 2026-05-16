#include "modules/Starter.hpp"
#include <chrono>

struct Vertex {
    glm::vec3 pos;
    glm::vec3 normal;
};

struct UniformBufferObject {
    alignas(16) glm::mat4 mvpMat;
    alignas(16) glm::mat4 modelMat;
    alignas(16) glm::mat4 normalMat;
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

    // Scene Objects
    Model M_Cube;
    DescriptorSet DS_Cube;
    DescriptorSet DS_Global;

    // Camera State
    float Ar;
    float camYaw = 0.0f;
    float camPitch = 0.0f;
    float camDist = 3.0f;

    void setWindowParameters() {
        windowWidth = 800;
        windowHeight = 600;
        windowTitle = "Dungeon Tavern - Cube Scene";
        windowResizable = GLFW_TRUE;
        initialBackgroundColor = {0.05f, 0.05f, 0.1f, 1.0f};
    }

    void onWindowResize(int w, int h) {
        RP.width = w;
        RP.height = h;
        Ar = (float)w / (float)h;
    }

    void localInit() {
        // 1. Descriptor Layouts
        DSL_Global.init(this, {
            {0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS, sizeof(GlobalUniformBufferObject), 1}
        });

        DSL_Object.init(this, {
            {0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT, sizeof(UniformBufferObject), 1}
        });

        // 2. Vertex Descriptor
        VD.init(this,
            {{0, sizeof(Vertex), VK_VERTEX_INPUT_RATE_VERTEX}},
            {
                {0, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, pos), sizeof(glm::vec3), POSITION},
                {0, 1, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, normal), sizeof(glm::vec3), NORMAL}
            }
        );

        // 3. Render Pass & Pipeline
        RP.init(this);
        RP.properties[0].clearValue = {0.05f, 0.05f, 0.1f, 1.0f};

        P.init(this, &VD, "shaders/shader.vert.spv", "shaders/shader.frag.spv", {&DSL_Global, &DSL_Object});
        P.setCullMode(VK_CULL_MODE_BACK_BIT);

        // 4. Geometry Definition
        std::vector<Vertex> cubeVerts = {
            // Front (+Z)
            {{-0.5f, -0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}}, {{ 0.5f, -0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}},
            {{ 0.5f,  0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}}, {{-0.5f,  0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}},
            // Back (-Z)
            {{ 0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}}, {{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}},
            {{-0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}}, {{ 0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}},
            // Right (+X)
            {{ 0.5f, -0.5f,  0.5f}, {1.0f, 0.0f, 0.0f}}, {{ 0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
            {{ 0.5f,  0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}}, {{ 0.5f,  0.5f,  0.5f}, {1.0f, 0.0f, 0.0f}},
            // Left (-X)
            {{-0.5f, -0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f}}, {{-0.5f, -0.5f,  0.5f}, {-1.0f, 0.0f, 0.0f}},
            {{-0.5f,  0.5f,  0.5f}, {-1.0f, 0.0f, 0.0f}}, {{-0.5f,  0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f}},
            // Top (+Y)
            {{-0.5f,  0.5f,  0.5f}, {0.0f, 1.0f, 0.0f}}, {{ 0.5f,  0.5f,  0.5f}, {0.0f, 1.0f, 0.0f}},
            {{ 0.5f,  0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}}, {{-0.5f,  0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
            // Bottom (-Y)
            {{-0.5f, -0.5f, -0.5f}, {0.0f, -1.0f, 0.0f}}, {{ 0.5f, -0.5f, -0.5f}, {0.0f, -1.0f, 0.0f}},
            {{ 0.5f, -0.5f,  0.5f}, {0.0f, -1.0f, 0.0f}}, {{-0.5f, -0.5f,  0.5f}, {0.0f, -1.0f, 0.0f}}
        };

        M_Cube.vertices = std::vector<unsigned char>(
            reinterpret_cast<unsigned char*>(cubeVerts.data()),
            reinterpret_cast<unsigned char*>(cubeVerts.data()) + cubeVerts.size() * sizeof(Vertex)
        );

        M_Cube.indices = {
            0, 1, 2,  2, 3, 0,      4, 5, 6,  6, 7, 4,
            8, 9, 10, 10, 11, 8,    12, 13, 14, 14, 15, 12,
            16, 17, 18, 18, 19, 16, 20, 21, 22, 22, 23, 20
        };
        M_Cube.initMesh(this, &VD);

        // 5. Descriptor Pool Config
        DPSZs.uniformBlocksInPool = 2;
        DPSZs.setsInPool = 2;

        Ar = (float)windowWidth / (float)windowHeight;
        submitCommandBuffer("main", 0, populateCommandBufferAccess, this);
    }

    void pipelinesAndDescriptorSetsInit() {
        RP.create();
        P.create(&RP);
        DS_Global.init(this, &DSL_Global, {});
        DS_Cube.init(this, &DSL_Object, {});
    }

    void pipelinesAndDescriptorSetsCleanup() {
        P.cleanup();
        RP.cleanup();
        DS_Global.cleanup();
        DS_Cube.cleanup();
    }

    void localCleanup() {
        M_Cube.cleanup();
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
        DS_Cube.bind(commandBuffer, P, 1, currentImage);

        M_Cube.bind(commandBuffer);
        vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(M_Cube.indices.size()), 1, 0, 0, 0);

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
        const float ROT_SPEED = glm::radians(90.0f);
        if (glfwGetKey(window, GLFW_KEY_A)) camYaw   -= ROT_SPEED * deltaT;
        if (glfwGetKey(window, GLFW_KEY_D)) camYaw   += ROT_SPEED * deltaT;
        if (glfwGetKey(window, GLFW_KEY_W)) camPitch += ROT_SPEED * deltaT;
        if (glfwGetKey(window, GLFW_KEY_S)) camPitch -= ROT_SPEED * deltaT;

        camPitch = glm::clamp(camPitch, glm::radians(-85.0f), glm::radians(85.0f));

        // Matrices
        glm::vec3 cameraPos = glm::vec3(
            camDist * cos(camPitch) * sin(camYaw),
            camDist * sin(camPitch),
            camDist * cos(camPitch) * cos(camYaw)
        );

        glm::mat4 view = glm::lookAt(cameraPos, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 proj = glm::perspective(glm::radians(45.0f), Ar, 0.1f, 100.0f);
        proj[1][1] *= -1;

        // Update UBOs
        UniformBufferObject ubo{};
        ubo.modelMat = glm::mat4(1.0f);
        ubo.mvpMat = proj * view * ubo.modelMat;
        ubo.normalMat = glm::inverse(glm::transpose(ubo.modelMat));
        DS_Cube.map(currentImage, &ubo, 0);

        GlobalUniformBufferObject gubo{};
        gubo.lightDir = glm::normalize(glm::vec3(-0.5f, -1.0f, -0.3f));
        gubo.lightColor = glm::vec4(1.0f);
        gubo.eyePos = cameraPos;
        DS_Global.map(currentImage, &gubo, 0);
    }
};

int main() {
    try { DungeonTavern().run(); }
    catch (const std::exception& e) { std::cerr << e.what() << std::endl; return 1; }
    return 0;
}