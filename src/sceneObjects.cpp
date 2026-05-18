#include "sceneObjects.hpp"

void SceneObjects::loadAll(BaseProject* bp, VertexDescriptor* VD) {
    // Models: loaded from file
    M_Character.init(bp, VD, "assets/models/character1.obj", OBJ);
    M_Table.init(bp, VD, "assets/models/table.obj", OBJ);
    M_Character2.init(bp, VD, "assets/models/character2.obj", OBJ);
    M_Fire.init(bp, VD, "assets/models/fireplace.obj", OBJ);
    M_Bar.init(bp, VD, "assets/models/bar.obj", OBJ);
    M_Bar2.init(bp, VD, "assets/models/bar2.obj", OBJ);

    //Textures
    T_Character.init(bp, "assets/textures/character1.png");
    T_Character2.init(bp, "assets/textures/character2.jpg");
    T_Table.init(bp, "assets/textures/table2.png");
    T_Fire.init(bp, "assets/textures/fireplace_Albedo.png");
    T_Floor.init(bp, "assets/textures/floor.jpg");
    T_Bar.init(bp, "assets/textures/BeerBar_Base_color_1001.png");
    T_Bar2.init(bp, "assets/textures/Bar.png");


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
    DS_Character2.init(bp, DSL_Object, { T_Character2.getViewAndSampler() });
    DS_Table_A.init(bp, DSL_Object, { T_Table.getViewAndSampler() });
    DS_Table_B.init(bp, DSL_Object, { T_Table.getViewAndSampler() });
    DS_Table_C.init(bp, DSL_Object, { T_Table.getViewAndSampler() });
    DS_Fire.init(bp, DSL_Object, { T_Fire.getViewAndSampler() });
    DS_Floor.init(bp, DSL_Object, { T_Floor.getViewAndSampler() });
    DS_Bar.init(bp, DSL_Object, { T_Bar.getViewAndSampler() });
    DS_Bar2.init(bp, DSL_Object, { T_Bar2.getViewAndSampler() });
}

void SceneObjects::cleanupDescriptorSets() {
    DS_Character.cleanup();
    DS_Character2.cleanup();
    DS_Table_A.cleanup();
    DS_Table_B.cleanup();
    DS_Table_C.cleanup();
    DS_Fire.cleanup();
    DS_Floor.cleanup();
    DS_Bar.cleanup();
    DS_Bar2.cleanup();
}

void SceneObjects::cleanupAll() {
    //Models
    M_Character.cleanup();
    M_Character2.cleanup();
    M_Table.cleanup();
    M_Floor.cleanup();
    M_Fire.cleanup();
    M_Bar.cleanup();
    M_Bar2.cleanup();
    //Textures
    T_Character.cleanup();
    T_Character2.cleanup();
    T_Table.cleanup();
    T_Fire.cleanup();
    T_Floor.cleanup();
    T_Bar.cleanup();
    T_Bar2.cleanup();
}

void SceneObjects::drawAll(VkCommandBuffer cb, Pipeline& P, int currentImage) {
    // Character 1
    DS_Character.bind(cb, P, 1, currentImage);
    M_Character.bind(cb);
    vkCmdDrawIndexed(cb,
        static_cast<uint32_t>(M_Character.indices.size()), 1, 0, 0, 0);
    // Character 2
    DS_Character2.bind(cb, P, 1, currentImage);
    M_Character2.bind(cb);
    vkCmdDrawIndexed(cb,
        static_cast<uint32_t>(M_Character2.indices.size()), 1, 0, 0, 0);
    // Table A
    DS_Table_A.bind(cb, P, 1, currentImage);
    M_Table.bind(cb);
    vkCmdDrawIndexed(cb,
        static_cast<uint32_t>(M_Table.indices.size()), 1, 0, 0, 0);

    // Table B
    DS_Table_B.bind(cb, P, 1, currentImage);
    M_Table.bind(cb);
    vkCmdDrawIndexed(cb, static_cast<uint32_t>(M_Table.indices.size()), 1, 0, 0, 0);

    // Table C
    DS_Table_C.bind(cb, P, 1, currentImage);
    M_Table.bind(cb);
    vkCmdDrawIndexed(cb, static_cast<uint32_t>(M_Table.indices.size()), 1, 0, 0, 0);

    // Floor
    DS_Floor.bind(cb, P, 1, currentImage);
    M_Floor.bind(cb);
    vkCmdDrawIndexed(cb,
        static_cast<uint32_t>(M_Floor.indices.size()), 1, 0, 0, 0);
    // Fireplace
    DS_Fire.bind(cb, P, 1, currentImage);
    M_Fire.bind(cb);
    vkCmdDrawIndexed(cb, static_cast<uint32_t>(M_Fire.indices.size()), 1, 0, 0, 0);

    // Bar
    DS_Bar.bind(cb, P, 1, currentImage);
    M_Bar.bind(cb);
    vkCmdDrawIndexed(cb, static_cast<uint32_t>(M_Bar.indices.size()), 1, 0, 0, 0);
    // Bar 2
    DS_Bar2.bind(cb, P, 1, currentImage);
    M_Bar2.bind(cb);
    vkCmdDrawIndexed(cb, static_cast<uint32_t>(M_Bar2.indices.size()), 1, 0, 0, 0);


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
          glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, CHAR_Y_OFFSET, 2.0f))
        * glm::rotate   (glm::mat4(1.0f), npcRotation, glm::vec3(0.0f, 1.0f, 0.0f))
        * glm::scale    (glm::mat4(1.0f), glm::vec3(CHAR_SCALE));

    UniformBufferObject charUbo{};
    charUbo.modelMat  = charModel;
    charUbo.mvpMat    = proj * view * charModel;
    charUbo.normalMat = glm::inverse(glm::transpose(charModel));
    DS_Character.map(currentImage, &charUbo, 0);

    // ----- Character 2 -----
    constexpr float CHAR2_SCALE    = 1.3f;
    constexpr float CHAR2_Y_OFFSET = 0.0f;

    glm::mat4 char2Model =
          glm::translate(glm::mat4(1.0f), glm::vec3(-2.0f, CHAR2_Y_OFFSET, -2.0f))
        * glm::scale    (glm::mat4(1.0f), glm::vec3(CHAR2_SCALE));

    UniformBufferObject char2Ubo{};
    char2Ubo.modelMat  = char2Model;
    char2Ubo.mvpMat    = proj * view * char2Model;
    char2Ubo.normalMat = glm::inverse(glm::transpose(char2Model));
    DS_Character2.map(currentImage, &char2Ubo, 0);

    // ----- TABLES
    constexpr float TABLE_SCALE    = 0.013f;

    constexpr float TABLE_Y_OFFSET = 0.0f;

    // ----- Table A -----
    glm::mat4 tableModel =
          glm::translate(glm::mat4(1.0f), glm::vec3(5.0f, TABLE_Y_OFFSET, -4.0f))
        * glm::scale    (glm::mat4(1.0f), glm::vec3(TABLE_SCALE));

    UniformBufferObject tableUbo{};
    tableUbo.modelMat  = tableModel;
    tableUbo.mvpMat    = proj * view * tableModel;
    tableUbo.normalMat = glm::inverse(glm::transpose(tableModel));
    DS_Table_A.map(currentImage, &tableUbo, 0);

    // ----- Table B  -----
    glm::mat4 tableBModel =
          glm::translate(glm::mat4(1.0f), glm::vec3(-5.0f, TABLE_Y_OFFSET, -4.0f))
        * glm::scale    (glm::mat4(1.0f), glm::vec3(TABLE_SCALE));

    UniformBufferObject tableBUbo{};
    tableBUbo.modelMat  = tableBModel;
    tableBUbo.mvpMat    = proj * view * tableBModel;
    tableBUbo.normalMat = glm::inverse(glm::transpose(tableBModel));
    DS_Table_B.map(currentImage, &tableBUbo, 0);

    // ----- Table C -----
    glm::mat4 tableCModel =
          glm::translate(glm::mat4(1.0f), glm::vec3(5.0f, TABLE_Y_OFFSET, 4.0f))
        * glm::scale    (glm::mat4(1.0f), glm::vec3(TABLE_SCALE));

    UniformBufferObject tableCUbo{};
    tableCUbo.modelMat  = tableCModel;
    tableCUbo.mvpMat    = proj * view * tableCModel;
    tableCUbo.normalMat = glm::inverse(glm::transpose(tableCModel));
    DS_Table_C.map(currentImage, &tableCUbo, 0);

    // ----- Floor -----
    glm::mat4 floorModel = glm::mat4(1.0f);
    UniformBufferObject floorUbo{};
    floorUbo.modelMat  = floorModel;
    floorUbo.mvpMat    = proj * view * floorModel;
    floorUbo.normalMat = glm::inverse(glm::transpose(floorModel));
    DS_Floor.map(currentImage, &floorUbo, 0);

    // ----- Fireplace -----
    constexpr float FIRE_SCALE    = 0.05f;
    constexpr float FIRE_Y_OFFSET = 0.0f;

    glm::mat4 fireModel =
          glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, FIRE_Y_OFFSET, -9.0f))
        * glm::scale    (glm::mat4(1.0f), glm::vec3(FIRE_SCALE));

    UniformBufferObject fireUbo{};
    fireUbo.modelMat  = fireModel;
    fireUbo.mvpMat    = proj * view * fireModel;
    fireUbo.normalMat = glm::inverse(glm::transpose(fireModel));
    DS_Fire.map(currentImage, &fireUbo, 0);

    // ----- Bar -----
    constexpr float BAR_SCALE    = 0.008f;
    constexpr float BAR_Y_OFFSET = 1.5f;

    glm::mat4 barModel =
          glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, BAR_Y_OFFSET, 0.0f))
        * glm::scale    (glm::mat4(1.0f), glm::vec3(BAR_SCALE));

    UniformBufferObject barUbo{};
    barUbo.modelMat  = barModel;
    barUbo.mvpMat    = proj * view * barModel;
    barUbo.normalMat = glm::inverse(glm::transpose(barModel));
    DS_Bar.map(currentImage, &barUbo, 0);

    // ----- Bar 2 -----
    constexpr float BAR2_SCALE    = 0.8f;
    constexpr float BAR2_Y_OFFSET = 0.0f;

    glm::mat4 bar2Model =
          glm::translate(glm::mat4(1.0f), glm::vec3(-6.0f, BAR2_Y_OFFSET, 6.0f))
        * glm::rotate   (glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)) // ⬇️ FIXED: Semicolon removed!
        * glm::scale    (glm::mat4(1.0f), glm::vec3(BAR2_SCALE));

    UniformBufferObject bar2Ubo{};
    bar2Ubo.modelMat  = bar2Model;
    bar2Ubo.mvpMat    = proj * view * bar2Model;
    bar2Ubo.normalMat = glm::inverse(glm::transpose(bar2Model));
    DS_Bar2.map(currentImage, &bar2Ubo, 0);
}