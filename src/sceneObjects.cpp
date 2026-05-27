#include "sceneObjects.hpp"
#include <cmath>

void SceneObjects::loadAll(BaseProject* bp, VertexDescriptor* VD) {
    // Models loaded from asset files
    M_Innkeeper.init(bp, VD, "assets/models/innkeeper.obj", OBJ);
    M_Merchant.init(bp, VD, "assets/models/merchant.obj", OBJ);
    M_SeatedNpc.init(bp, VD, "assets/models/seated_npc.obj", OBJ);
    M_Orc.init(bp, VD, "assets/models/orc.obj", OBJ);

    M_Table.init(bp, VD, "assets/models/table.obj", OBJ);
    M_Fireplace.init(bp, VD, "assets/models/fireplace.obj", OBJ);
    M_BarCounter.init(bp, VD, "assets/models/bar_counter.obj", OBJ);
    M_BackBar.init(bp, VD, "assets/models/back_bar.obj", OBJ);
    M_WallTorch.init(bp, VD, "assets/models/wall_torch.obj", OBJ);
    M_Door.init(bp, VD, "assets/models/door.obj", OBJ);
    M_HangingChain.init(bp, VD, "assets/models/hanging_chain.obj", OBJ);

    // Textures loaded from asset files.
    T_Innkeeper.init(bp, "assets/textures/innkeeper_albedo.png");
    T_Merchant.init(bp, "assets/textures/merchant_albedo.png");
    T_SeatedNpc.init(bp, "assets/textures/seated_npc_albedo.png");
    T_Orc.init(bp, "assets/textures/orc_albedo.png");

    T_Table.init(bp, "assets/textures/table_albedo.jpeg");
    T_Fireplace.init(bp, "assets/textures/fireplace_albedo.png");
    T_BarCounter.init(bp, "assets/textures/bar_counter_albedo.png");
    T_BackBar.init(bp, "assets/textures/back_bar_albedo.png");
    T_WallTorch.init(bp, "assets/textures/wall_torch_albedo.png");
    T_Door.init(bp, "assets/textures/door_albedo.png");
    T_HangingChain.init(bp, "assets/textures/hanging_chain_albedo.png");

    T_Floor.init(bp, "assets/textures/room_floor.jpg");
    T_Wall.init(bp, "assets/textures/room_wall.jpg");
    T_Ceiling.init(bp, "assets/textures/room_ceiling.jpg");

    // Repeated decorative objects: chains and torches
    constexpr float ROOM_HALF_T = 10.0f;

    chainPositions = {
        {-ROOM_HALF_T + 0.1f, 1.5f,  1.9f},
        {-ROOM_HALF_T + 0.1f, 1.5f, -1.9f},
        { ROOM_HALF_T - 0.1f, 1.5f,  1.9f},
        { ROOM_HALF_T - 0.1f, 1.5f, -1.9f},
        { 5.0f, 1.5f, -ROOM_HALF_T + 0.1f},
        {-5.0f, 1.5f, -ROOM_HALF_T + 0.1f},
        { 5.0f, 1.5f,  ROOM_HALF_T - 0.1f},
        {-5.0f, 1.5f,  ROOM_HALF_T - 0.1f},
    };

    chainRot = {
        90.0f, 90.0f, 90.0f, 90.0f,
        90.0f, 90.0f, 90.0f, 90.0f
    };

    chainWallSpin = {
        0.0f, 0.0f, 0.0f, 0.0f,
        90.0f, 90.0f, 90.0f, 90.0f
    };

    torchPositions = {
        {-ROOM_HALF_T + 0.1f, 2.5f,  4.0f},
        {-ROOM_HALF_T + 0.1f, 2.5f, -4.0f},
        { ROOM_HALF_T - 0.1f, 2.5f,  4.0f},
        { ROOM_HALF_T - 0.1f, 2.5f, -4.0f},
    };

    torchYaws = {
        90.0f, 90.0f,
        -90.0f, -90.0f
    };

    // Room geometry: floor, walls, and ceiling.
    constexpr float ROOM_HALF = 10.0f;
    constexpr float TILE = 5.0f;

    // Floor: procedural quad on the XZ-plane at y = 0
    // Normals point upward and texture coordinates are tiled
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

    // Walls
    constexpr float WALL_H = 4.0f;
    constexpr float WALL_TILE_X = 3.0f;
    constexpr float WALL_TILE_Y = 1.0f;

    // North wall: z = +ROOM_HALF, facing inward toward -Z.
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

    // South wall: z = -ROOM_HALF, facing inward toward +Z
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

    // East wall: x = +ROOM_HALF, facing inward toward -X
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

    // West wall: x = -ROOM_HALF, facing inward toward +X
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

    // Ceiling: y = WALL_H, facing downward toward -Y
    constexpr float CEIL_TILE = 5.0f;

    std::vector<SceneVertex> scene_vertices = {
        {{-ROOM_HALF, WALL_H, -ROOM_HALF}, {0.0f, -1.0f, 0.0f}, {0.0f,      0.0f     }},
        {{ ROOM_HALF, WALL_H, -ROOM_HALF}, {0.0f, -1.0f, 0.0f}, {CEIL_TILE, 0.0f     }},
        {{ ROOM_HALF, WALL_H,  ROOM_HALF}, {0.0f, -1.0f, 0.0f}, {CEIL_TILE, CEIL_TILE}},
        {{-ROOM_HALF, WALL_H,  ROOM_HALF}, {0.0f, -1.0f, 0.0f}, {0.0f,      CEIL_TILE}},
    };

    M_Ceiling.vertices = std::vector<unsigned char>(
        reinterpret_cast<unsigned char*>(scene_vertices.data()),
        reinterpret_cast<unsigned char*>(scene_vertices.data()) + scene_vertices.size() * sizeof(SceneVertex)
    );

    M_Ceiling.indices = {0, 1, 2,  0, 2, 3};
    M_Ceiling.initMesh(bp, VD);
}

void SceneObjects::registerColliders(CollisionSystem& collisionSystem) const {
    collisionSystem.clear();
    collisionSystem.setPlayerRadius(0.35f);

    // Room bounds
    // The visible room extends from -10 to +10 in both X and Z
    collisionSystem.setRoomBounds(
        -10.0f, 10.0f,
        -10.0f, 10.0f
    );

    // NPC colliders
    // Circle colliders are used because the NPCs are mostly vertical

    // Innkeeper
    collisionSystem.addCircleCollider(glm::vec2(2.0f, 2.0f), 0.20f);

    // Merchant
    collisionSystem.addCircleCollider(glm::vec2(6.1f, 1.0f), 0.20f);

    // Orc
    collisionSystem.addCircleCollider(glm::vec2(-6.3f, 7.3f), 0.20f);

    // Tables
    collisionSystem.addBoxCollider(
        glm::vec2(5.0f, -4.0f),
        glm::vec2(1.2f, 1.2f)
    );

    collisionSystem.addBoxCollider(
        glm::vec2(-5.0f, -4.0f),
        glm::vec2(1.2f, 1.2f)
    );

    collisionSystem.addBoxCollider(
        glm::vec2(5.0f, 4.0f),
        glm::vec2(1.2f, 1.2f)
    );

    // Fireplace
    collisionSystem.addBoxCollider(
        glm::vec2(0.0f, -9.0f),
        glm::vec2(1.60f, 0.60f)
    );

    // Bar counter
    collisionSystem.addBoxCollider(
        glm::vec2(0.0f, 0.0f),
        glm::vec2(1.00f, 3.00f)
    );

    // Back bar

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
    // Descriptor sets connect each rendered object to its texture
    DS_Innkeeper.init(bp, DSL_Object, { T_Innkeeper.getViewAndSampler() });
    DS_Merchant.init(bp, DSL_Object, { T_Merchant.getViewAndSampler() });
    DS_SeatedNpc.init(bp, DSL_Object, { T_SeatedNpc.getViewAndSampler() });
    DS_Orc.init(bp, DSL_Object, { T_Orc.getViewAndSampler() });

    DS_Table_A.init(bp, DSL_Object, { T_Table.getViewAndSampler() });
    DS_Table_B.init(bp, DSL_Object, { T_Table.getViewAndSampler() });
    DS_Table_C.init(bp, DSL_Object, { T_Table.getViewAndSampler() });

    DS_Fireplace.init(bp, DSL_Object, { T_Fireplace.getViewAndSampler() });
    DS_BarCounter.init(bp, DSL_Object, { T_BarCounter.getViewAndSampler() });
    DS_BackBar.init(bp, DSL_Object, { T_BackBar.getViewAndSampler() });
    DS_Door.init(bp, DSL_Object, { T_Door.getViewAndSampler() });

    // Room descriptor sets
    DS_Floor.init(bp, DSL_Object, { T_Floor.getViewAndSampler() });
    DS_WallN.init(bp, DSL_Object, { T_Wall.getViewAndSampler() });
    DS_WallS.init(bp, DSL_Object, { T_Wall.getViewAndSampler() });
    DS_WallE.init(bp, DSL_Object, { T_Wall.getViewAndSampler() });
    DS_WallW.init(bp, DSL_Object, { T_Wall.getViewAndSampler() });
    DS_Ceiling.init(bp, DSL_Object, { T_Ceiling.getViewAndSampler() });

    // One descriptor set per torch instance
    DS_WallTorches.resize(torchPositions.size());
    for (auto& ds : DS_WallTorches) {
        ds.init(bp, DSL_Object, { T_WallTorch.getViewAndSampler() });
    }

    // One descriptor set per hanging chain instance
    DS_HangingChains.resize(chainPositions.size());
    for (auto& ds : DS_HangingChains) {
        ds.init(bp, DSL_Object, { T_HangingChain.getViewAndSampler() });
    }
}

void SceneObjects::cleanupDescriptorSets() {
    // Model descriptor sets.
    DS_Innkeeper.cleanup();
    DS_Merchant.cleanup();
    DS_SeatedNpc.cleanup();
    DS_Orc.cleanup();

    DS_Table_A.cleanup();
    DS_Table_B.cleanup();
    DS_Table_C.cleanup();

    DS_Fireplace.cleanup();
    DS_BarCounter.cleanup();
    DS_BackBar.cleanup();
    DS_Door.cleanup();

    for (auto& ds : DS_WallTorches) {
        ds.cleanup();
    }
    DS_WallTorches.clear();

    for (auto& ds : DS_HangingChains) {
        ds.cleanup();
    }
    DS_HangingChains.clear();

    // Room descriptor sets
    DS_Floor.cleanup();
    DS_WallN.cleanup();
    DS_WallS.cleanup();
    DS_WallE.cleanup();
    DS_WallW.cleanup();
    DS_Ceiling.cleanup();
}

void SceneObjects::cleanupAll() {
    // Models
    M_Innkeeper.cleanup();
    M_Merchant.cleanup();
    M_SeatedNpc.cleanup();
    M_Orc.cleanup();

    M_Table.cleanup();
    M_Fireplace.cleanup();
    M_BarCounter.cleanup();
    M_BackBar.cleanup();
    M_WallTorch.cleanup();
    M_Door.cleanup();
    M_HangingChain.cleanup();

    // Room meshes
    M_Floor.cleanup();
    M_WallN.cleanup();
    M_WallS.cleanup();
    M_WallE.cleanup();
    M_WallW.cleanup();
    M_Ceiling.cleanup();

    // Textures
    T_Innkeeper.cleanup();
    T_Merchant.cleanup();
    T_SeatedNpc.cleanup();
    T_Orc.cleanup();

    T_Table.cleanup();
    T_Fireplace.cleanup();
    T_BarCounter.cleanup();
    T_BackBar.cleanup();
    T_WallTorch.cleanup();
    T_Door.cleanup();
    T_HangingChain.cleanup();

    T_Floor.cleanup();
    T_Wall.cleanup();
    T_Ceiling.cleanup();
}

void SceneObjects::drawAll(VkCommandBuffer cb, Pipeline& P, int currentImage) {
    // Draw all imported models

    // Innkeeper
    DS_Innkeeper.bind(cb, P, 1, currentImage);
    M_Innkeeper.bind(cb);
    vkCmdDrawIndexed(cb, static_cast<uint32_t>(M_Innkeeper.indices.size()), 1, 0, 0, 0);

    // Merchant
    DS_Merchant.bind(cb, P, 1, currentImage);
    M_Merchant.bind(cb);
    vkCmdDrawIndexed(cb, static_cast<uint32_t>(M_Merchant.indices.size()), 1, 0, 0, 0);

    // Seated NPC
    DS_SeatedNpc.bind(cb, P, 1, currentImage);
    M_SeatedNpc.bind(cb);
    vkCmdDrawIndexed(cb, static_cast<uint32_t>(M_SeatedNpc.indices.size()), 1, 0, 0, 0);

    // Tables
    DS_Table_A.bind(cb, P, 1, currentImage);
    M_Table.bind(cb);
    vkCmdDrawIndexed(cb, static_cast<uint32_t>(M_Table.indices.size()), 1, 0, 0, 0);

    DS_Table_B.bind(cb, P, 1, currentImage);
    M_Table.bind(cb);
    vkCmdDrawIndexed(cb, static_cast<uint32_t>(M_Table.indices.size()), 1, 0, 0, 0);

    DS_Table_C.bind(cb, P, 1, currentImage);
    M_Table.bind(cb);
    vkCmdDrawIndexed(cb, static_cast<uint32_t>(M_Table.indices.size()), 1, 0, 0, 0);

    // Fireplace
    DS_Fireplace.bind(cb, P, 1, currentImage);
    M_Fireplace.bind(cb);
    vkCmdDrawIndexed(cb, static_cast<uint32_t>(M_Fireplace.indices.size()), 1, 0, 0, 0);

    // Wall torches
    M_WallTorch.bind(cb);
    for (auto& ds : DS_WallTorches) {
        ds.bind(cb, P, 1, currentImage);
        vkCmdDrawIndexed(cb, static_cast<uint32_t>(M_WallTorch.indices.size()), 1, 0, 0, 0);
    }

    // Bar counter
    DS_BarCounter.bind(cb, P, 1, currentImage);
    M_BarCounter.bind(cb);
    vkCmdDrawIndexed(cb, static_cast<uint32_t>(M_BarCounter.indices.size()), 1, 0, 0, 0);

    // Back bar
    DS_BackBar.bind(cb, P, 1, currentImage);
    M_BackBar.bind(cb);
    vkCmdDrawIndexed(cb, static_cast<uint32_t>(M_BackBar.indices.size()), 1, 0, 0, 0);

    // Orc
    DS_Orc.bind(cb, P, 1, currentImage);
    M_Orc.bind(cb);
    vkCmdDrawIndexed(cb, static_cast<uint32_t>(M_Orc.indices.size()), 1, 0, 0, 0);

    // Door
    DS_Door.bind(cb, P, 1, currentImage);
    M_Door.bind(cb);
    vkCmdDrawIndexed(cb, static_cast<uint32_t>(M_Door.indices.size()), 1, 0, 0, 0);

    // Hanging chains
    M_HangingChain.bind(cb);
    for (auto& ds : DS_HangingChains) {
        ds.bind(cb, P, 1, currentImage);
        vkCmdDrawIndexed(cb, static_cast<uint32_t>(M_HangingChain.indices.size()), 1, 0, 0, 0);
    }

    // Draw procedural room geometry
    DS_Floor.bind(cb, P, 1, currentImage);
    M_Floor.bind(cb);
    vkCmdDrawIndexed(cb, static_cast<uint32_t>(M_Floor.indices.size()), 1, 0, 0, 0);

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
}

void SceneObjects::updateUBOs(int currentImage,
                              const glm::mat4& proj,
                              const glm::mat4& view,
                              int activeNpcIndex,
                              const glm::vec3& playerPosition) {
    // Innkeeper
    // The innkeeper rotates to face the player when he is the active NPC
    constexpr float INNKEEPER_SCALE = 0.01f;
    constexpr float INNKEEPER_Y_OFFSET = 0.0f;
    const glm::vec3 innkeeperPosition = glm::vec3(2.0f, INNKEEPER_Y_OFFSET, 2.0f);

    float innkeeperRotation = 0.0f;

    if (activeNpcIndex == 0) {
        glm::vec3 directionToPlayer = playerPosition - innkeeperPosition;
        directionToPlayer.y = 0.0f;

        if (glm::length(directionToPlayer) > 0.0001f) {
            directionToPlayer = glm::normalize(directionToPlayer);

            // The model's default forward direction appears to be +Z, so atan2 computes the yaw needed to face the player.
            innkeeperRotation = std::atan2(directionToPlayer.x, directionToPlayer.z);
        }
    }

    glm::mat4 innkeeperModel =
          glm::translate(glm::mat4(1.0f), innkeeperPosition)
        * glm::rotate(glm::mat4(1.0f), innkeeperRotation, glm::vec3(0.0f, 1.0f, 0.0f))
        * glm::scale(glm::mat4(1.0f), glm::vec3(INNKEEPER_SCALE));

    UniformBufferObject innkeeperUbo{};
    innkeeperUbo.modelMat = innkeeperModel;
    innkeeperUbo.mvpMat = proj * view * innkeeperModel;
    innkeeperUbo.normalMat = glm::inverse(glm::transpose(innkeeperModel));
    DS_Innkeeper.map(currentImage, &innkeeperUbo, 0);

    // Door
    constexpr float DOOR_SCALE = 1.5f;

    glm::mat4 doorModel =
          glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 10.0f))
        * glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f))
        * glm::scale(glm::mat4(1.0f), glm::vec3(DOOR_SCALE));

    UniformBufferObject doorUbo{};
    doorUbo.modelMat = doorModel;
    doorUbo.mvpMat = proj * view * doorModel;
    doorUbo.normalMat = glm::inverse(glm::transpose(doorModel));
    DS_Door.map(currentImage, &doorUbo, 0);

    // Wall torches
    constexpr float WALL_TORCH_SCALE = 0.3f;

    for (size_t i = 0; i < DS_WallTorches.size(); ++i) {
        glm::mat4 wallTorchModel =
              glm::translate(glm::mat4(1.0f), torchPositions[i])
            * glm::rotate(glm::mat4(1.0f), glm::radians(torchYaws[i]), glm::vec3(0.0f, 1.0f, 0.0f))
            * glm::scale(glm::mat4(1.0f), glm::vec3(WALL_TORCH_SCALE));

        UniformBufferObject wallTorchUbo{};
        wallTorchUbo.modelMat = wallTorchModel;
        wallTorchUbo.mvpMat = proj * view * wallTorchModel;
        wallTorchUbo.normalMat = glm::inverse(glm::transpose(wallTorchModel));
        DS_WallTorches[i].map(currentImage, &wallTorchUbo, 0);
    }


    // Hanging chains
    constexpr float HANGING_CHAIN_SCALE = 1.25f;

    for (size_t i = 0; i < DS_HangingChains.size(); ++i) {
        glm::mat4 hangingChainModel =
              glm::translate(glm::mat4(1.0f), chainPositions[i])
            * glm::rotate(glm::mat4(1.0f), glm::radians(chainWallSpin[i]), glm::vec3(0.0f, 1.0f, 0.0f))
            * glm::rotate(glm::mat4(1.0f), glm::radians(chainRot[i]), glm::vec3(1.0f, 0.0f, 0.0f))
            * glm::rotate(glm::mat4(1.0f), glm::radians(chainRot[i]), glm::vec3(0.0f, 0.0f, 1.0f))
            * glm::scale(glm::mat4(1.0f), glm::vec3(HANGING_CHAIN_SCALE));

        UniformBufferObject hangingChainUbo{};
        hangingChainUbo.modelMat = hangingChainModel;
        hangingChainUbo.mvpMat = proj * view * hangingChainModel;
        hangingChainUbo.normalMat = glm::inverse(glm::transpose(hangingChainModel));
        DS_HangingChains[i].map(currentImage, &hangingChainUbo, 0);
    }

    // Room: meshes are already created in world space, so they use identity as their model matrix.
    glm::mat4 identityModel = glm::mat4(1.0f);

    UniformBufferObject roomUbo{};
    roomUbo.modelMat = identityModel;
    roomUbo.mvpMat = proj * view * identityModel;
    roomUbo.normalMat = glm::inverse(glm::transpose(identityModel));

    DS_Floor.map(currentImage, &roomUbo, 0);
    DS_WallN.map(currentImage, &roomUbo, 0);
    DS_WallS.map(currentImage, &roomUbo, 0);
    DS_WallE.map(currentImage, &roomUbo, 0);
    DS_WallW.map(currentImage, &roomUbo, 0);
    DS_Ceiling.map(currentImage, &roomUbo, 0);

    // Merchant
    constexpr float MERCHANT_SCALE = 1.15f;
    constexpr float MERCHANT_Y_OFFSET = 0.0f;

    glm::mat4 merchantModel =
          glm::translate(glm::mat4(1.0f), glm::vec3(6.1f, MERCHANT_Y_OFFSET, 1.0f))
        * glm::rotate(glm::mat4(1.0f), glm::radians(-50.0f), glm::vec3(0.0f, 1.0f, 0.0f))
        * glm::scale(glm::mat4(1.0f), glm::vec3(MERCHANT_SCALE));

    UniformBufferObject merchantUbo{};
    merchantUbo.modelMat = merchantModel;
    merchantUbo.mvpMat = proj * view * merchantModel;
    merchantUbo.normalMat = glm::inverse(glm::transpose(merchantModel));
    DS_Merchant.map(currentImage, &merchantUbo, 0);

    // Orc
    // The orc rotates to face the player when he is the active NPC
    constexpr float ORC_SCALE = 1.0f;
    constexpr float ORC_Y_OFFSET = 0.0f;
    const glm::vec3 orcPosition = glm::vec3(-6.3f, ORC_Y_OFFSET, 7.3f);

    float orcRotation = glm::radians(180.0f);

    if (activeNpcIndex == 1) {
        glm::vec3 orcDirectionToPlayer = playerPosition - orcPosition;
        orcDirectionToPlayer.y = 0.0f;

        if (glm::length(orcDirectionToPlayer) > 0.0001f) {
            orcDirectionToPlayer = glm::normalize(orcDirectionToPlayer);
            orcRotation = std::atan2(orcDirectionToPlayer.x, orcDirectionToPlayer.z);
        }
    }

    glm::mat4 orcModel =
          glm::translate(glm::mat4(1.0f), orcPosition)
        * glm::rotate(glm::mat4(1.0f), orcRotation, glm::vec3(0.0f, 1.0f, 0.0f))
        * glm::scale(glm::mat4(1.0f), glm::vec3(ORC_SCALE));

    UniformBufferObject orcUbo{};
    orcUbo.modelMat = orcModel;
    orcUbo.mvpMat = proj * view * orcModel;
    orcUbo.normalMat = glm::inverse(glm::transpose(orcModel));
    DS_Orc.map(currentImage, &orcUbo, 0);

    // Seated NPC
    constexpr float SEATED_NPC_SCALE = 1.4f;
    constexpr float SEATED_NPC_Y_OFFSET = 0.0f;

    glm::mat4 seatedNpcModel =
          glm::translate(glm::mat4(1.0f), glm::vec3(3.87f, SEATED_NPC_Y_OFFSET, 3.5f))
        * glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f))
        * glm::scale(glm::mat4(1.0f), glm::vec3(SEATED_NPC_SCALE));

    UniformBufferObject seatedNpcUbo{};
    seatedNpcUbo.modelMat = seatedNpcModel;
    seatedNpcUbo.mvpMat = proj * view * seatedNpcModel;
    seatedNpcUbo.normalMat = glm::inverse(glm::transpose(seatedNpcModel));
    DS_SeatedNpc.map(currentImage, &seatedNpcUbo, 0);

    // Tables
    // The same table model is reused three times with different positions
    constexpr float TABLE_SCALE = 0.13f;
    constexpr float TABLE_Y_OFFSET = 0.0f;

    glm::mat4 tableAModel =
          glm::translate(glm::mat4(1.0f), glm::vec3(5.0f, TABLE_Y_OFFSET, -4.0f))
        * glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f))
        * glm::scale(glm::mat4(1.0f), glm::vec3(TABLE_SCALE));

    UniformBufferObject tableAUbo{};
    tableAUbo.modelMat = tableAModel;
    tableAUbo.mvpMat = proj * view * tableAModel;
    tableAUbo.normalMat = glm::inverse(glm::transpose(tableAModel));
    DS_Table_A.map(currentImage, &tableAUbo, 0);

    glm::mat4 tableBModel =
          glm::translate(glm::mat4(1.0f), glm::vec3(-5.0f, TABLE_Y_OFFSET, -4.0f))
        * glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f))
        * glm::scale(glm::mat4(1.0f), glm::vec3(TABLE_SCALE));

    UniformBufferObject tableBUbo{};
    tableBUbo.modelMat = tableBModel;
    tableBUbo.mvpMat = proj * view * tableBModel;
    tableBUbo.normalMat = glm::inverse(glm::transpose(tableBModel));
    DS_Table_B.map(currentImage, &tableBUbo, 0);

    glm::mat4 tableCModel =
          glm::translate(glm::mat4(1.0f), glm::vec3(5.0f, TABLE_Y_OFFSET, 4.0f))
        * glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f))
        * glm::scale(glm::mat4(1.0f), glm::vec3(TABLE_SCALE));

    UniformBufferObject tableCUbo{};
    tableCUbo.modelMat = tableCModel;
    tableCUbo.mvpMat = proj * view * tableCModel;
    tableCUbo.normalMat = glm::inverse(glm::transpose(tableCModel));
    DS_Table_C.map(currentImage, &tableCUbo, 0);

    // Fireplace
    constexpr float FIREPLACE_SCALE = 0.05f;
    constexpr float FIREPLACE_Y_OFFSET = 0.0f;

    glm::mat4 fireplaceModel =
          glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, FIREPLACE_Y_OFFSET, -9.0f))
        * glm::scale(glm::mat4(1.0f), glm::vec3(FIREPLACE_SCALE));

    UniformBufferObject fireplaceUbo{};
    fireplaceUbo.modelMat = fireplaceModel;
    fireplaceUbo.mvpMat = proj * view * fireplaceModel;
    fireplaceUbo.normalMat = glm::inverse(glm::transpose(fireplaceModel));
    DS_Fireplace.map(currentImage, &fireplaceUbo, 0);

    // Bar counter
    constexpr float BAR_COUNTER_SCALE = 0.007f;
    constexpr float BAR_COUNTER_Y_OFFSET = 1.4f;

    glm::mat4 barCounterModel =
          glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, BAR_COUNTER_Y_OFFSET, 0.0f))
        * glm::scale(glm::mat4(1.0f), glm::vec3(BAR_COUNTER_SCALE));

    UniformBufferObject barCounterUbo{};
    barCounterUbo.modelMat = barCounterModel;
    barCounterUbo.mvpMat = proj * view * barCounterModel;
    barCounterUbo.normalMat = glm::inverse(glm::transpose(barCounterModel));
    DS_BarCounter.map(currentImage, &barCounterUbo, 0);

    // Back bar
    constexpr float BACK_BAR_SCALE = 0.6f;
    constexpr float BACK_BAR_Y_OFFSET = 0.0f;

    glm::mat4 backBarModel =
          glm::translate(glm::mat4(1.0f), glm::vec3(-6.0f, BACK_BAR_Y_OFFSET, 6.0f))
        * glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f))
        * glm::scale(glm::mat4(1.0f), glm::vec3(BACK_BAR_SCALE));

    UniformBufferObject backBarUbo{};
    backBarUbo.modelMat = backBarModel;
    backBarUbo.mvpMat = proj * view * backBarModel;
    backBarUbo.normalMat = glm::inverse(glm::transpose(backBarModel));
    DS_BackBar.map(currentImage, &backBarUbo, 0);
}