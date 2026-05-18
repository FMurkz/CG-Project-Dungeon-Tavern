#include "sceneObjects.hpp"

void SceneObjects::loadAll(BaseProject* bp, VertexDescriptor* VD) {
    // Models: loaded from file
    M_Character.init(bp, VD, "assets/models/character1.obj", OBJ);

    //Textures
    T_Character.init(bp, "assets/textures/character1.png");
    T_Floor.init(bp, "assets/textures/floor.jpg");
    T_Wall   .init(bp, "assets/textures/wall.jpg");
    T_Ceiling.init(bp, "assets/textures/ceiling.png");


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
{
    std::vector<SceneVertex> verts = {
        {{-ROOM_HALF, 0.0f,    ROOM_HALF}, {0.0f, 0.0f, -1.0f}, {0.0f,        0.0f       }},
        {{ ROOM_HALF, 0.0f,    ROOM_HALF}, {0.0f, 0.0f, -1.0f}, {WALL_TILE_X, 0.0f       }},
        {{ ROOM_HALF, WALL_H,  ROOM_HALF}, {0.0f, 0.0f, -1.0f}, {WALL_TILE_X, WALL_TILE_Y}},
        {{-ROOM_HALF, WALL_H,  ROOM_HALF}, {0.0f, 0.0f, -1.0f}, {0.0f,        WALL_TILE_Y}},
    };
    M_WallN.vertices = std::vector<unsigned char>(
        reinterpret_cast<unsigned char*>(verts.data()),
        reinterpret_cast<unsigned char*>(verts.data()) + verts.size() * sizeof(SceneVertex)
    );
    M_WallN.indices = {0, 2, 1,  0, 3, 2};
    M_WallN.initMesh(bp, VD);
}

// South wall: at z = -ROOM_HALF, faces +Z (inward)
{
    std::vector<SceneVertex> verts = {
        {{ ROOM_HALF, 0.0f,   -ROOM_HALF}, {0.0f, 0.0f, 1.0f}, {0.0f,        0.0f       }},
        {{-ROOM_HALF, 0.0f,   -ROOM_HALF}, {0.0f, 0.0f, 1.0f}, {WALL_TILE_X, 0.0f       }},
        {{-ROOM_HALF, WALL_H, -ROOM_HALF}, {0.0f, 0.0f, 1.0f}, {WALL_TILE_X, WALL_TILE_Y}},
        {{ ROOM_HALF, WALL_H, -ROOM_HALF}, {0.0f, 0.0f, 1.0f}, {0.0f,        WALL_TILE_Y}},
    };
    M_WallS.vertices = std::vector<unsigned char>(
        reinterpret_cast<unsigned char*>(verts.data()),
        reinterpret_cast<unsigned char*>(verts.data()) + verts.size() * sizeof(SceneVertex)
    );
    M_WallS.indices = {0, 2, 1,  0, 3, 2};
    M_WallS.initMesh(bp, VD);
}

// East wall: at x = +ROOM_HALF, faces -X (inward)
{
    std::vector<SceneVertex> verts = {
        {{ ROOM_HALF, 0.0f,    ROOM_HALF}, {-1.0f, 0.0f, 0.0f}, {0.0f,        0.0f       }},
        {{ ROOM_HALF, 0.0f,   -ROOM_HALF}, {-1.0f, 0.0f, 0.0f}, {WALL_TILE_X, 0.0f       }},
        {{ ROOM_HALF, WALL_H, -ROOM_HALF}, {-1.0f, 0.0f, 0.0f}, {WALL_TILE_X, WALL_TILE_Y}},
        {{ ROOM_HALF, WALL_H,  ROOM_HALF}, {-1.0f, 0.0f, 0.0f}, {0.0f,        WALL_TILE_Y}},
    };
    M_WallE.vertices = std::vector<unsigned char>(
        reinterpret_cast<unsigned char*>(verts.data()),
        reinterpret_cast<unsigned char*>(verts.data()) + verts.size() * sizeof(SceneVertex)
    );
    M_WallE.indices = {0, 2, 1,  0, 3, 2};
    M_WallE.initMesh(bp, VD);
}

// West wall: at x = -ROOM_HALF, faces +X (inward)
{
    std::vector<SceneVertex> verts = {
        {{-ROOM_HALF, 0.0f,   -ROOM_HALF}, {1.0f, 0.0f, 0.0f}, {0.0f,        0.0f       }},
        {{-ROOM_HALF, 0.0f,    ROOM_HALF}, {1.0f, 0.0f, 0.0f}, {WALL_TILE_X, 0.0f       }},
        {{-ROOM_HALF, WALL_H,  ROOM_HALF}, {1.0f, 0.0f, 0.0f}, {WALL_TILE_X, WALL_TILE_Y}},
        {{-ROOM_HALF, WALL_H, -ROOM_HALF}, {1.0f, 0.0f, 0.0f}, {0.0f,        WALL_TILE_Y}},
    };
    M_WallW.vertices = std::vector<unsigned char>(
        reinterpret_cast<unsigned char*>(verts.data()),
        reinterpret_cast<unsigned char*>(verts.data()) + verts.size() * sizeof(SceneVertex)
    );
    M_WallW.indices = {0, 2, 1,  0, 3, 2};
    M_WallW.initMesh(bp, VD);
}

    // Ceiling: at y = WALL_H, faces down (-Y)
    {
        constexpr float CEIL_TILE = 5.0f;
        std::vector<SceneVertex> verts = {
            {{-ROOM_HALF, WALL_H, -ROOM_HALF}, {0.0f, -1.0f, 0.0f}, {0.0f,      0.0f     }},
            {{ ROOM_HALF, WALL_H, -ROOM_HALF}, {0.0f, -1.0f, 0.0f}, {CEIL_TILE, 0.0f     }},
            {{ ROOM_HALF, WALL_H,  ROOM_HALF}, {0.0f, -1.0f, 0.0f}, {CEIL_TILE, CEIL_TILE}},
            {{-ROOM_HALF, WALL_H,  ROOM_HALF}, {0.0f, -1.0f, 0.0f}, {0.0f,      CEIL_TILE}},
        };
        M_Ceiling.vertices = std::vector<unsigned char>(
            reinterpret_cast<unsigned char*>(verts.data()),
            reinterpret_cast<unsigned char*>(verts.data()) + verts.size() * sizeof(SceneVertex)
        );

        M_Ceiling.indices = {0, 1, 2,  0, 2, 3};

        M_Ceiling.initMesh(bp, VD);
    }
}

void SceneObjects::initDescriptorSets(BaseProject* bp, DescriptorSetLayout* DSL_Object) {
    DS_Character.init(bp, DSL_Object, { T_Character.getViewAndSampler() });
    DS_Floor    .init(bp, DSL_Object, { T_Floor    .getViewAndSampler() });

    DS_WallN    .init(bp, DSL_Object, { T_Wall     .getViewAndSampler() });
    DS_WallS    .init(bp, DSL_Object, { T_Wall     .getViewAndSampler() });
    DS_WallE    .init(bp, DSL_Object, { T_Wall     .getViewAndSampler() });
    DS_WallW    .init(bp, DSL_Object, { T_Wall     .getViewAndSampler() });
    DS_Ceiling  .init(bp, DSL_Object, { T_Ceiling  .getViewAndSampler() });
}

void SceneObjects::cleanupDescriptorSets() {
    DS_Character.cleanup();
    DS_Floor.cleanup();

    DS_WallN.cleanup();
    DS_WallS.cleanup();
    DS_WallE.cleanup();
    DS_WallW.cleanup();
    DS_Ceiling.cleanup();
}

void SceneObjects::cleanupAll() {
    //Models
    M_Character.cleanup();
    M_Floor.cleanup();

    M_WallN.cleanup();
    M_WallS.cleanup();
    M_WallE.cleanup();
    M_WallW.cleanup();
    M_Ceiling.cleanup();

    // Textures
    T_Character.cleanup();
    T_Floor.cleanup();
    T_Wall.cleanup();
    T_Ceiling.cleanup();
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

    glm::mat4 IdentityModel = glm::mat4(1.0f);
    UniformBufferObject roomUbo{};
    roomUbo.modelMat  = IdentityModel;
    roomUbo.mvpMat    = proj * view * IdentityModel;
    roomUbo.normalMat = glm::inverse(glm::transpose(IdentityModel));

    // Map the shared matrix state out to all structural uniform slots
    DS_Floor.map(currentImage, &roomUbo, 0);
    DS_WallN.map(currentImage, &roomUbo, 0);
    DS_WallS.map(currentImage, &roomUbo, 0);
    DS_WallE.map(currentImage, &roomUbo, 0);
    DS_WallW.map(currentImage, &roomUbo, 0);
    DS_Ceiling.map(currentImage, &roomUbo, 0);
}