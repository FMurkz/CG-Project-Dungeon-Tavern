#include "sceneObjects.hpp"
#include <cmath>

void SceneObjects::loadAll(BaseProject* bp, VertexDescriptor* VD) {
    //==============================================================
    //                  Models: loaded from file
    M_Character.init(bp, VD, "assets/models/character1.obj", OBJ);
    M_Table.init(bp, VD, "assets/models/table.obj", OBJ);
    M_Character2.init(bp, VD, "assets/models/character2.obj", OBJ);
    M_Fire.init(bp, VD, "assets/models/fireplace.obj", OBJ);
    M_Bar.init(bp, VD, "assets/models/bar.obj", OBJ);
    M_Bar2.init(bp, VD, "assets/models/bar2.obj", OBJ);
    M_Torch.init(bp, VD, "assets/models/torch.obj", OBJ);
    M_Orc.init(bp, VD, "assets/models/orc.obj", OBJ);
    M_Sitting.init(bp, VD, "assets/models/sitting.obj", OBJ);
    M_Door.init(bp, VD, "assets/models/doors.obj", OBJ);

    //                         Textures
    T_Character.init(bp, "assets/textures/character1.png");
    T_Character2.init(bp, "assets/textures/character2.jpg");
    T_Table.init(bp, "assets/textures/table.jpg");
    T_Fire.init(bp, "assets/textures/fireplace_Albedo.png");
    T_Floor.init(bp, "assets/textures/floor.jpg");
    T_Bar.init(bp, "assets/textures/BeerBar_Base_color_1001.png");
    T_Bar2.init(bp, "assets/textures/Bar.png");
    T_Wall   .init(bp, "assets/textures/wall.jpg");
    T_Torch.init(bp, "assets/textures/torchColor.png");
    T_Ceiling.init(bp, "assets/textures/ceiling.jpg");
    T_Orc.init(bp, "assets/textures/Orc.png");
    T_Sitting.init(bp, "assets/textures/Sitting.png");
    T_Door.init(bp, "assets/textures/doors.png");
    //============================================================

    constexpr float ROOM_HALF_T = 10.0f;
    torchPositions = {
        {-ROOM_HALF_T + 0.1f, 2.5f,  4.0f},
        {-ROOM_HALF_T + 0.1f, 2.5f, -4.0f},
        { ROOM_HALF_T - 0.1f, 2.5f,  4.0f},
        { ROOM_HALF_T - 0.1f, 2.5f, -4.0f},
    };
    torchYaws = { 90.0f, 90.0f, -90.0f, -90.0f };

    //==========================================================================
    //                          ROOM: Floor, Walls, Ceiling
    // Floor: procedural quad on the XZ plane at y = 0, normals pointing up
    constexpr float ROOM_HALF = 10.0f;
    constexpr float TILE = 5.0f;
    std::vector<SceneVertex> floorVerts = {
        {{-ROOM_HALF, 0.0f, -ROOM_HALF}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
        {{ ROOM_HALF, 0.0f, -ROOM_HALF}, {0.0f, 1.0f, 0.0f}, {TILE, 0.0f}},
        {{ ROOM_HALF, 0.0f,  ROOM_HALF}, {0.0f, 1.0f, 0.0f}, {TILE, TILE}},
        {{-ROOM_HALF, 0.0f,  ROOM_HALF}, {0.0f, 1.0f, 0.0f}, {0.0f, TILE}},
    };
    M_Floor.vertices = std::vector<unsigned char>(
        reinterpret_cast<unsigned char*>(floorVerts.data()),
        reinterpret_cast<unsigned char*>(floorVerts.data())
            + floorVerts.size() * sizeof(SceneVertex)
    );
    M_Floor.indices = {0, 2, 1,  0, 3, 2};
    M_Floor.initMesh(bp, VD);

    // --- Walls ---
    constexpr float WALL_H = 4.0f;
    constexpr float WALL_TILE_X = 3.0f;    // texture repeats horizontally
    constexpr float WALL_TILE_Y = 1.0f;    // texture repeats vertically

    // North wall: at z = +ROOM_HALF, faces -Z (inward)
    std::vector<SceneVertex> vertsN = {
        {{-ROOM_HALF, 0.0f,    ROOM_HALF}, {0.0f, 0.0f, -1.0f}, {0.0f,        0.0f       }},
        {{ ROOM_HALF, 0.0f,    ROOM_HALF}, {0.0f, 0.0f, -1.0f}, {WALL_TILE_X, 0.0f       }},
        {{ ROOM_HALF, WALL_H,  ROOM_HALF}, {0.0f, 0.0f, -1.0f}, {WALL_TILE_X, WALL_TILE_Y}},
        {{-ROOM_HALF, WALL_H,  ROOM_HALF}, {0.0f, 0.0f, -1.0f}, {0.0f,        WALL_TILE_Y}},
    };
    M_WallN.vertices = std::vector<unsigned char>(
        reinterpret_cast<unsigned char*>(vertsN.data()),
        reinterpret_cast<unsigned char*>(vertsN.data()) + vertsN.size() * sizeof(SceneVertex)
    );
    M_WallN.indices = {0, 2, 1,  0, 3, 2};
    M_WallN.initMesh(bp, VD);

    // South wall: at z = -ROOM_HALF, faces +Z (inward)
    std::vector<SceneVertex> vertsS = {
        {{ ROOM_HALF, 0.0f,   -ROOM_HALF}, {0.0f, 0.0f, 1.0f}, {0.0f,        0.0f       }},
        {{-ROOM_HALF, 0.0f,   -ROOM_HALF}, {0.0f, 0.0f, 1.0f}, {WALL_TILE_X, 0.0f       }},
        {{-ROOM_HALF, WALL_H, -ROOM_HALF}, {0.0f, 0.0f, 1.0f}, {WALL_TILE_X, WALL_TILE_Y}},
        {{ ROOM_HALF, WALL_H, -ROOM_HALF}, {0.0f, 0.0f, 1.0f}, {0.0f,        WALL_TILE_Y}},
    };
    M_WallS.vertices = std::vector<unsigned char>(
        reinterpret_cast<unsigned char*>(vertsS.data()),
        reinterpret_cast<unsigned char*>(vertsS.data()) + vertsS.size() * sizeof(SceneVertex)
    );
    M_WallS.indices = {0, 2, 1,  0, 3, 2};
    M_WallS.initMesh(bp, VD);

    // East wall: at x = +ROOM_HALF, faces -X (inward)
    std::vector<SceneVertex> vertsE = {
        {{ ROOM_HALF, 0.0f,    ROOM_HALF}, {-1.0f, 0.0f, 0.0f}, {0.0f,        0.0f       }},
        {{ ROOM_HALF, 0.0f,   -ROOM_HALF}, {-1.0f, 0.0f, 0.0f}, {WALL_TILE_X, 0.0f       }},
        {{ ROOM_HALF, WALL_H, -ROOM_HALF}, {-1.0f, 0.0f, 0.0f}, {WALL_TILE_X, WALL_TILE_Y}},
        {{ ROOM_HALF, WALL_H,  ROOM_HALF}, {-1.0f, 0.0f, 0.0f}, {0.0f,        WALL_TILE_Y}},
    };
    M_WallE.vertices = std::vector<unsigned char>(
        reinterpret_cast<unsigned char*>(vertsE.data()),
        reinterpret_cast<unsigned char*>(vertsE.data()) + vertsE.size() * sizeof(SceneVertex)
    );
    M_WallE.indices = {0, 2, 1,  0, 3, 2};
    M_WallE.initMesh(bp, VD);

    // West wall: at x = -ROOM_HALF, faces +X (inward)
    std::vector<SceneVertex> vertsW = {
        {{-ROOM_HALF, 0.0f,   -ROOM_HALF}, {1.0f, 0.0f, 0.0f}, {0.0f,        0.0f       }},
        {{-ROOM_HALF, 0.0f,    ROOM_HALF}, {1.0f, 0.0f, 0.0f}, {WALL_TILE_X, 0.0f       }},
        {{-ROOM_HALF, WALL_H,  ROOM_HALF}, {1.0f, 0.0f, 0.0f}, {WALL_TILE_X, WALL_TILE_Y}},
        {{-ROOM_HALF, WALL_H, -ROOM_HALF}, {1.0f, 0.0f, 0.0f}, {0.0f,        WALL_TILE_Y}},
    };
    M_WallW.vertices = std::vector<unsigned char>(
        reinterpret_cast<unsigned char*>(vertsW.data()),
        reinterpret_cast<unsigned char*>(vertsW.data()) + vertsW.size() * sizeof(SceneVertex)
    );
    M_WallW.indices = {0, 2, 1,  0, 3, 2};
    M_WallW.initMesh(bp, VD);

    // Ceiling: at y = WALL_H, faces down (-Y)

    constexpr float CEIL_TILE = 5.0f;
    std::vector<SceneVertex> vertsC = {
        {{-ROOM_HALF, WALL_H, -ROOM_HALF}, {0.0f, -1.0f, 0.0f}, {0.0f,      0.0f     }},
        {{ ROOM_HALF, WALL_H, -ROOM_HALF}, {0.0f, -1.0f, 0.0f}, {CEIL_TILE, 0.0f     }},
        {{ ROOM_HALF, WALL_H,  ROOM_HALF}, {0.0f, -1.0f, 0.0f}, {CEIL_TILE, CEIL_TILE}},
        {{-ROOM_HALF, WALL_H,  ROOM_HALF}, {0.0f, -1.0f, 0.0f}, {0.0f,      CEIL_TILE}},
    };
    M_Ceiling.vertices = std::vector<unsigned char>(
        reinterpret_cast<unsigned char*>(vertsC.data()),
        reinterpret_cast<unsigned char*>(vertsC.data()) + vertsC.size() * sizeof(SceneVertex)
    );

    M_Ceiling.indices = {0, 1, 2,  0, 2, 3};

    M_Ceiling.initMesh(bp, VD);
    //=====================================================
}

void SceneObjects::registerColliders(CollisionSystem& collisionSystem) const {
    collisionSystem.clear();
    collisionSystem.setPlayerRadius(0.35f);

    // ============================================================
    // ROOM BOUNDS
    // The camera/player is not allowed to leave the room.
    // Room extends visually from -10 to +10 in X and Z.
    // ============================================================
    collisionSystem.setRoomBounds(
        -10.0f, 10.0f,
        -10.0f, 10.0f
    );

    // ============================================================
    // NPCs
    // ============================================================
    collisionSystem.addCircleCollider(
        glm::vec2(0.0f, 0.0f),
        0.40f
    );

    collisionSystem.addCircleCollider(
        glm::vec2(-2.0f, -2.0f),
        0.40f
    );

    // Orc / bartender
    collisionSystem.addCircleCollider(
        glm::vec2(-6.3f, 7.3f),
        0.40f
    );

    // ============================================================
    // TABLES
    // Positions match updateUBOs()
    // ============================================================
    collisionSystem.addBoxCollider(
        glm::vec2(5.0f, -4.0f),
        glm::vec2(2.00f, 2.00f)
    );

    collisionSystem.addBoxCollider(
        glm::vec2(-5.0f, -4.0f),
        glm::vec2(2.00f, 2.00f)
    );

    collisionSystem.addBoxCollider(
        glm::vec2(5.0f, 4.0f),
        glm::vec2(2.00f, 2.00f)
    );

    // ============================================================
    // FIREPLACE
    // ============================================================
    collisionSystem.addBoxCollider(
        glm::vec2(0.0f, -9.0f),
        glm::vec2(1.60f, 0.60f)
    );

    // ============================================================
    // CENTRAL BAR
    // At the moment this is approximate and may need tuning
    // after you test it visually.
    // ============================================================
    collisionSystem.addBoxCollider(
        glm::vec2(0.0f, 0.0f),
        glm::vec2(1.00f, 3.00f)
    );

    // ============================================================
    // SECOND BAR
    // Bar2 is rotated 90 degrees in updateUBOs()
    // ============================================================
    // Horizontal side of the L-shaped bar
    collisionSystem.addBoxCollider(
        glm::vec2(-6.55f, 5.85f),
        glm::vec2(2.45f, 0.65f)
    );

    // Vertical side of the L-shaped bar
    collisionSystem.addBoxCollider(
        glm::vec2(-4.70f, 7.20f),
        glm::vec2(0.65f, 1.85f)
    );
}

void SceneObjects::initDescriptorSets(BaseProject* bp, DescriptorSetLayout* DSL_Object) {
    //Models
    DS_Character.init(bp, DSL_Object, { T_Character.getViewAndSampler() });
    DS_Character2.init(bp, DSL_Object, { T_Character2.getViewAndSampler() });
    DS_Table_A.init(bp, DSL_Object, { T_Table.getViewAndSampler() });
    DS_Table_B.init(bp, DSL_Object, { T_Table.getViewAndSampler() });
    DS_Table_C.init(bp, DSL_Object, { T_Table.getViewAndSampler() });
    DS_Fire.init(bp, DSL_Object, { T_Fire.getViewAndSampler() });
    DS_Bar.init(bp, DSL_Object, { T_Bar.getViewAndSampler() });
    DS_Bar2.init(bp, DSL_Object, { T_Bar2.getViewAndSampler() });
    DS_Orc.init(bp, DSL_Object, { T_Orc.getViewAndSampler() });
    DS_Sitting.init(bp, DSL_Object, { T_Sitting.getViewAndSampler() });
    DS_Door.init(bp, DSL_Object, { T_Door.getViewAndSampler() });
    //Room
    DS_Floor.init(bp, DSL_Object, { T_Floor.getViewAndSampler() });
    DS_WallN    .init(bp, DSL_Object, { T_Wall     .getViewAndSampler() });
    DS_WallS    .init(bp, DSL_Object, { T_Wall     .getViewAndSampler() });
    DS_WallE    .init(bp, DSL_Object, { T_Wall     .getViewAndSampler() });
    DS_WallW    .init(bp, DSL_Object, { T_Wall     .getViewAndSampler() });
    DS_Ceiling  .init(bp, DSL_Object, { T_Ceiling  .getViewAndSampler() });

    DS_Torches.resize(torchPositions.size());
    for (auto& ds : DS_Torches) {
        ds.init(bp, DSL_Object, { T_Torch.getViewAndSampler() });
    }
}

void SceneObjects::cleanupDescriptorSets() {
    //Models
    DS_Character.cleanup();
    DS_Character2.cleanup();
    DS_Table_A.cleanup();
    DS_Table_B.cleanup();
    DS_Table_C.cleanup();
    DS_Fire.cleanup();
    DS_Floor.cleanup();
    DS_Orc.cleanup();
    DS_Sitting.cleanup();
    DS_Door.cleanup();
    for (auto& ds : DS_Torches) ds.cleanup();
    DS_Torches.clear();

    //Room
    DS_WallN.cleanup();
    DS_WallS.cleanup();
    DS_WallE.cleanup();
    DS_WallW.cleanup();
    DS_Ceiling.cleanup();
    DS_Bar.cleanup();
    DS_Bar2.cleanup();
}

void SceneObjects::cleanupAll() {
    //Models
    M_Character.cleanup();
    M_Character2.cleanup();
    M_Table.cleanup();
    M_Fire.cleanup();
    M_Bar.cleanup();
    M_Bar2.cleanup();
    M_Orc.cleanup();
    M_Sitting.cleanup();
    M_Torch.cleanup();
    M_Door.cleanup();

    //Room
    M_Floor.cleanup();
    M_WallN.cleanup();
    M_WallS.cleanup();
    M_WallE.cleanup();
    M_WallW.cleanup();
    M_Ceiling.cleanup();


    //Textures
    T_Character.cleanup();
    T_Character2.cleanup();
    T_Table.cleanup();
    T_Fire.cleanup();
    T_Floor.cleanup();
    T_Bar.cleanup();
    T_Bar2.cleanup();
    T_Wall.cleanup();
    T_Ceiling.cleanup();
    T_Torch.cleanup();
    T_Orc.cleanup();
    T_Sitting.cleanup();
    T_Door.cleanup();
}

void SceneObjects::drawAll(VkCommandBuffer cb, Pipeline& P, int currentImage) {
    //Models
    DS_Character.bind(cb, P, 1, currentImage);
    M_Character.bind(cb);
    vkCmdDrawIndexed(cb,
        static_cast<uint32_t>(M_Character.indices.size()), 1, 0, 0, 0);
    // Character 2
    DS_Character2.bind(cb, P, 1, currentImage);
    M_Character2.bind(cb);
    vkCmdDrawIndexed(cb,
        static_cast<uint32_t>(M_Character2.indices.size()), 1, 0, 0, 0);
    // Sitting Character
    DS_Sitting.bind(cb, P, 1, currentImage);
    M_Sitting.bind(cb);
    vkCmdDrawIndexed(cb,
        static_cast<uint32_t>(M_Sitting.indices.size()), 1, 0, 0, 0);
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

   // Fireplace
    DS_Fire.bind(cb, P, 1, currentImage);
    M_Fire.bind(cb);
    vkCmdDrawIndexed(cb, static_cast<uint32_t>(M_Fire.indices.size()), 1, 0, 0, 0);
    //-------Torches------------
    M_Torch.bind(cb);
    for (auto& ds : DS_Torches) {
        ds.bind(cb, P, 1, currentImage);
        vkCmdDrawIndexed(cb, static_cast<uint32_t>(M_Torch.indices.size()), 1, 0, 0, 0);
    }

    // Bar
    DS_Bar.bind(cb, P, 1, currentImage);
    M_Bar.bind(cb);
    vkCmdDrawIndexed(cb, static_cast<uint32_t>(M_Bar.indices.size()), 1, 0, 0, 0);
    // Bar 2
    DS_Bar2.bind(cb, P, 1, currentImage);
    M_Bar2.bind(cb);
    vkCmdDrawIndexed(cb, static_cast<uint32_t>(M_Bar2.indices.size()), 1, 0, 0, 0);
    // Orc
    DS_Orc.bind(cb, P, 1, currentImage);
    M_Orc.bind(cb);
    vkCmdDrawIndexed(cb, static_cast<uint32_t>(M_Orc.indices.size()), 1, 0, 0, 0);

    DS_Door.bind(cb, P, 1, currentImage);
    M_Door.bind(cb);
    vkCmdDrawIndexed(cb, static_cast<uint32_t>(M_Door.indices.size()), 1, 0, 0, 0);


    //====================================================================
    //                          ROOM

    DS_Floor.bind(cb, P, 1, currentImage);
    M_Floor.bind(cb);
    vkCmdDrawIndexed(cb,
        static_cast<uint32_t>(M_Floor.indices.size()), 1, 0, 0, 0);

    DS_WallN.bind(cb, P, 1, currentImage);
    M_WallN.bind(cb);
    vkCmdDrawIndexed(cb, static_cast<uint32_t>(M_WallN.indices.size()), 1, 0, 0, 0);

    DS_WallS.bind(cb, P, 1, currentImage);
    M_WallS.bind(cb);
    vkCmdDrawIndexed(cb, static_cast<uint32_t>(M_WallS.indices.size()), 1, 0, 0, 0);

    DS_WallE.bind(cb, P, 1, currentImage);
    M_WallE.bind(cb);
    vkCmdDrawIndexed(cb, static_cast<uint32_t>(M_WallE.indices.size()), 1, 0, 0, 0);

    DS_WallW.bind(cb, P, 1, currentImage);
    M_WallW.bind(cb);
    vkCmdDrawIndexed(cb, static_cast<uint32_t>(M_WallW.indices.size()), 1, 0, 0, 0);

    DS_Ceiling.bind(cb, P, 1, currentImage);
    M_Ceiling.bind(cb);
    vkCmdDrawIndexed(cb, static_cast<uint32_t>(M_Ceiling.indices.size()), 1, 0, 0, 0);
    //=======================================================================
}


void SceneObjects::updateUBOs(int currentImage,
                              const glm::mat4& proj,
                              const glm::mat4& view,
                              int activeNpcIndex,
                              const glm::vec3& playerPosition) {
    // ----- Character -----
    constexpr float CHAR_SCALE    = 0.01f;
    constexpr float CHAR_Y_OFFSET = 0.0f;
    const glm::vec3 npcPosition = glm::vec3(2.0f, CHAR_Y_OFFSET, 2.0f);

    float npcRotation = 0.0f;

    if (activeNpcIndex == 0) {
        // Direction from NPC to player, only in the horizontal XZ-plane
        glm::vec3 directionToPlayer = playerPosition - npcPosition;
        directionToPlayer.y = 0.0f;

        if (glm::length(directionToPlayer) > 0.0001f) {
            directionToPlayer = glm::normalize(directionToPlayer);

            // The model's default forward direction appears to be +Z,
            // so this computes the yaw needed to face the player.
            npcRotation = std::atan2(directionToPlayer.x, directionToPlayer.z);
        }
    }

    //-------Door---
    const float DOOR_SCALE = 1.5f;
    glm::mat4 doorModel =
            glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 10.0f))
            * glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f))
            * glm::scale(glm::mat4(1.0f), glm::vec3(DOOR_SCALE));

    //-----Torches-----
    const float TORCH_SCALE = 0.3f;
    for (size_t i = 0; i < DS_Torches.size(); ++i) {
        glm::mat4 m =
              glm::translate(glm::mat4(1.0f), torchPositions[i])
            * glm::rotate(glm::mat4(1.0f), glm::radians(torchYaws[i]), glm::vec3(0,1,0))
            * glm::scale(glm::mat4(1.0f), glm::vec3(TORCH_SCALE));
        UniformBufferObject u{};
        u.modelMat  = m;
        u.mvpMat    = proj * view * m;
        u.normalMat = glm::inverse(glm::transpose(m));
        DS_Torches[i].map(currentImage, &u, 0);
    }

    glm::mat4 charModel =
          glm::translate(glm::mat4(1.0f), npcPosition)
        * glm::rotate   (glm::mat4(1.0f), npcRotation, glm::vec3(0.0f, 1.0f, 0.0f))
        * glm::scale    (glm::mat4(1.0f), glm::vec3(CHAR_SCALE));

    UniformBufferObject charUbo{};
    charUbo.modelMat  = charModel;
    charUbo.mvpMat    = proj * view * charModel;
    charUbo.normalMat = glm::inverse(glm::transpose(charModel));
    DS_Character.map(currentImage, &charUbo, 0);

    glm::mat4 IdentityModel = glm::mat4(1.0f);
    UniformBufferObject roomUbo{};
    roomUbo.modelMat  = IdentityModel;
    roomUbo.mvpMat    = proj * view * IdentityModel;
    roomUbo.normalMat = glm::inverse(glm::transpose(IdentityModel));

    DS_Floor.map(currentImage, &roomUbo, 0);
    DS_WallN.map(currentImage, &roomUbo, 0);
    DS_WallS.map(currentImage, &roomUbo, 0);
    DS_WallE.map(currentImage, &roomUbo, 0);
    DS_WallW.map(currentImage, &roomUbo, 0);
    DS_Ceiling.map(currentImage, &roomUbo, 0);
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
    // ----- Orc -----
    constexpr float ORC_SCALE    = 1.0f;
    constexpr float ORC_Y_OFFSET = 0.0f;
    const glm::vec3 orcPosition  = glm::vec3(-6.3f, ORC_Y_OFFSET, 7.3f); // Extracted variable

    // Calculate dynamic rotation for Orc
    float orcRotation = glm::radians(180.0f); // Default resting rotation angle

    if (activeNpcIndex == 1) {
        // Direction from Orc to player, horizontal XZ-plane
        glm::vec3 orcDirectionToPlayer = playerPosition - orcPosition;
        orcDirectionToPlayer.y = 0.0f;

        if (glm::length(orcDirectionToPlayer) > 0.0001f) {
            orcDirectionToPlayer = glm::normalize(orcDirectionToPlayer);

            // Computes yaw to face player.
            // Note: If his model maps facing a different way natively,
            // you can add or subtract an offset like `+ glm::radians(180.0f)` here.
            orcRotation = std::atan2(orcDirectionToPlayer.x, orcDirectionToPlayer.z);
        }
    }

    glm::mat4 orcModel =
          glm::translate(glm::mat4(1.0f), orcPosition)
        * glm::rotate   (glm::mat4(1.0f), orcRotation, glm::vec3(0.0f, 1.0f, 0.0f)) // Dynamic rotation bound!
        * glm::scale    (glm::mat4(1.0f), glm::vec3(ORC_SCALE));

    UniformBufferObject orcUbo{};
    orcUbo.modelMat  = orcModel;
    orcUbo.mvpMat    = proj * view * orcModel;
    orcUbo.normalMat = glm::inverse(glm::transpose(orcModel));
    DS_Orc.map(currentImage, &orcUbo, 0);

    // ----- Sitting Character -----
    constexpr float SITTING_SCALE    = 1.4f;
    constexpr float SITTING_Y_OFFSET = 0.0f; //

    glm::mat4 sittingModel =
          glm::translate(glm::mat4(1.0f), glm::vec3(3.65f, SITTING_Y_OFFSET, 3.0f))
        * glm::rotate   (glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f))
        * glm::scale    (glm::mat4(1.0f), glm::vec3(SITTING_SCALE));

    UniformBufferObject sittingUbo{};
    sittingUbo.modelMat  = sittingModel;
    sittingUbo.mvpMat    = proj * view * sittingModel;
    sittingUbo.normalMat = glm::inverse(glm::transpose(sittingModel));
    DS_Sitting.map(currentImage, &sittingUbo, 0);

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
    constexpr float BAR_SCALE    = 0.007f;
    constexpr float BAR_Y_OFFSET = 1.4f;

    glm::mat4 barModel =
          glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, BAR_Y_OFFSET, 0.0f))
        * glm::scale    (glm::mat4(1.0f), glm::vec3(BAR_SCALE));

    UniformBufferObject barUbo{};
    barUbo.modelMat  = barModel;
    barUbo.mvpMat    = proj * view * barModel;
    barUbo.normalMat = glm::inverse(glm::transpose(barModel));
    DS_Bar.map(currentImage, &barUbo, 0);

    // ----- Bar 2 -----
    constexpr float BAR2_SCALE    = 0.6f;
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

    UniformBufferObject doorUbo{};
    doorUbo.modelMat  = doorModel;
    doorUbo.mvpMat    = proj * view * doorModel;
    doorUbo.normalMat = glm::inverse(glm::transpose(doorModel));
    DS_Door.map(currentImage, &doorUbo, 0);
}