#include "sceneObjects.hpp"

void SceneObjects::loadAll(BaseProject* bp, VertexDescriptor* VD) {
    // Models: loaded from file
    M_Character.init(bp, VD, "assets/models/character1.obj", OBJ);

    //Textures
    T_Character.init(bp, "assets/textures/character1.png");
    T_Floor.init(bp, "assets/textures/floor.jpg");


    // Floor: procedural quad on the XZ plane at y = 0, normals pointing up
    constexpr float FLOOR_SIZE = 10.0f;
    constexpr float TILE = 5.0f;
    std::vector<SceneVertex> floorVerts = {
        {{-FLOOR_SIZE, 0.0f, -FLOOR_SIZE}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
        {{ FLOOR_SIZE, 0.0f, -FLOOR_SIZE}, {0.0f, 1.0f, 0.0f}, {TILE, 0.0f}},
        {{ FLOOR_SIZE, 0.0f,  FLOOR_SIZE}, {0.0f, 1.0f, 0.0f}, {TILE, TILE}},
        {{-FLOOR_SIZE, 0.0f,  FLOOR_SIZE}, {0.0f, 1.0f, 0.0f}, {0.0f, TILE}},
    };
    M_Floor.vertices = std::vector<unsigned char>(
        reinterpret_cast<unsigned char*>(floorVerts.data()),
        reinterpret_cast<unsigned char*>(floorVerts.data())
            + floorVerts.size() * sizeof(SceneVertex)
    );
    M_Floor.indices = {0, 2, 1,  0, 3, 2};
    M_Floor.initMesh(bp, VD);
}

void SceneObjects::initDescriptorSets(BaseProject* bp, DescriptorSetLayout* DSL_Object) {
    DS_Character.init(bp, DSL_Object, { T_Character.getViewAndSampler() });
    DS_Floor    .init(bp, DSL_Object, { T_Floor    .getViewAndSampler() });
}

void SceneObjects::cleanupDescriptorSets() {
    DS_Character.cleanup();
    DS_Floor.cleanup();
}

void SceneObjects::cleanupAll() {
    //Models
    M_Character.cleanup();
    M_Floor.cleanup();
    //Textures
    T_Character.cleanup();
    T_Floor.cleanup();
}

void SceneObjects::drawAll(VkCommandBuffer cb, Pipeline& P, int currentImage) {
    DS_Character.bind(cb, P, 1, currentImage);
    M_Character.bind(cb);
    vkCmdDrawIndexed(cb,
        static_cast<uint32_t>(M_Character.indices.size()), 1, 0, 0, 0);

    DS_Floor.bind(cb, P, 1, currentImage);
    M_Floor.bind(cb);
    vkCmdDrawIndexed(cb,
        static_cast<uint32_t>(M_Floor.indices.size()), 1, 0, 0, 0);
}

void SceneObjects::updateUBOs(int currentImage,
                              const glm::mat4& proj,
                              const glm::mat4& view,
                              bool npcInteracted) {
    // ----- Character -----
    constexpr float CHAR_SCALE    = 0.01f;
    constexpr float CHAR_Y_OFFSET = 0.0f;
    float npcRotation = npcInteracted ? glm::radians(180.0f) : 0.0f;

    glm::mat4 charModel =
          glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, CHAR_Y_OFFSET, 0.0f))
        * glm::rotate   (glm::mat4(1.0f), npcRotation, glm::vec3(0.0f, 1.0f, 0.0f))
        * glm::scale    (glm::mat4(1.0f), glm::vec3(CHAR_SCALE));

    UniformBufferObject charUbo{};
    charUbo.modelMat  = charModel;
    charUbo.mvpMat    = proj * view * charModel;
    charUbo.normalMat = glm::inverse(glm::transpose(charModel));
    DS_Character.map(currentImage, &charUbo, 0);

    // ----- Floor -----
    glm::mat4 floorModel = glm::mat4(1.0f);
    UniformBufferObject floorUbo{};
    floorUbo.modelMat  = floorModel;
    floorUbo.mvpMat    = proj * view * floorModel;
    floorUbo.normalMat = glm::inverse(glm::transpose(floorModel));
    DS_Floor.map(currentImage, &floorUbo, 0);
}