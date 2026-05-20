#pragma once

#include "modules/Starter.hpp"
#include "CollisionSystem.hpp"

// Vertex must match the layout in main.cpp.
struct SceneVertex {
    glm::vec3 pos;
    glm::vec3 normal;
    glm::vec2 uv;
};

// Per-object uniform (must match shader's set 1, binding 0).
struct UniformBufferObject {
    alignas(16) glm::mat4 mvpMat;
    alignas(16) glm::mat4 modelMat;
    alignas(16) glm::mat4 normalMat;
};

// All the per-object scene data.
struct SceneObjects {
    //Characters and objects
    // ---- Models ----
    Model M_Character;
    Model M_Character2;
    Model M_Table;
    Model M_Fire;
    Model M_Bar;
    Model M_Bar2;
    Model M_Torch;
    Model M_Orc;
    Model M_Sitting;
    Model M_Door;


    // ---- Textures ----
    Texture T_Character;
    Texture T_Character2;
    Texture T_Table;
    Texture T_Fire;
    Texture T_Bar;
    Texture T_Bar2;
    Texture T_Torch;
    Texture T_Orc;
    Texture T_Sitting;
    Texture T_Door;

    // ---- Per-object descriptor sets ----
    DescriptorSet DS_Character;
    DescriptorSet DS_Character2;
    DescriptorSet DS_Table_A;
    DescriptorSet DS_Table_B;
    DescriptorSet DS_Table_C;
    DescriptorSet DS_Fire;
    DescriptorSet DS_Bar;
    DescriptorSet DS_Bar2;
    DescriptorSet DS_Orc;
    DescriptorSet DS_Sitting;
    DescriptorSet DS_Door;

    std::vector<DescriptorSet> DS_Torches;
    //Torch positions
    std::vector<glm::vec3>     torchPositions;
    std::vector<float>         torchYaws;


    // ---- Room: floor, 4 walls, ceiling ----
    Model         M_Floor,  M_WallN,  M_WallS,  M_WallE,  M_WallW,  M_Ceiling;
    DescriptorSet DS_Floor, DS_WallN, DS_WallS, DS_WallE, DS_WallW, DS_Ceiling;
    Texture       T_Floor,  T_Wall,   T_Ceiling;


    // Load all models. Call from localInit().
    void loadAll(BaseProject* bp, VertexDescriptor* VD);

    // Register colliders for all solid scene objects.
    void registerColliders(CollisionSystem& collisionSystem) const;

    // Create descriptor sets. Call from pipelinesAndDescriptorSetsInit().
    void initDescriptorSets(BaseProject* bp, DescriptorSetLayout* DSL_Object);

    // Release descriptor sets. Call from pipelinesAndDescriptorSetsCleanup().
    void cleanupDescriptorSets();

    // Release models. Call from localCleanup().
    void cleanupAll();

    // Record draw commands. Call from populateCommandBuffer().
    void drawAll(VkCommandBuffer cb, Pipeline& P, int currentImage);

    // Update every object's UBO with its transform. Call from
    // updateUniformBuffer(). proj and view come from main's camera.
    void updateUBOs(int currentImage,
                 const glm::mat4& proj,
                 const glm::mat4& view,
                 int activeNpcIndex,
                 const glm::vec3& playerPosition);

    // Number of objects. Used to size the descriptor pool.
    int count() const { return 21; }  //
};