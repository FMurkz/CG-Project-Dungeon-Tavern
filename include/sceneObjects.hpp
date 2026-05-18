#pragma once

#include "modules/Starter.hpp"

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

    // ---- Textures ----
    Texture T_Character;

    // ---- Per-object descriptor sets ----
    DescriptorSet DS_Character;


    // ---- Room: floor, 4 walls, ceiling ----
    Model         M_Floor,  M_WallN,  M_WallS,  M_WallE,  M_WallW,  M_Ceiling;
    DescriptorSet DS_Floor, DS_WallN, DS_WallS, DS_WallE, DS_WallW, DS_Ceiling;
    Texture       T_Floor,  T_Wall,   T_Ceiling;


    // Load all models. Call from localInit().
    void loadAll(BaseProject* bp, VertexDescriptor* VD);

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
    void updateUBOs(int currentImage, const glm::mat4& proj, const glm::mat4& view, bool npcInteracted);

    // Number of objects. Used to size the descriptor pool.
    int count() const { return 7; }  // character + floor + 4 walls + Ceiling
};