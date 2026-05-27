#pragma once

#include "modules/Starter.hpp"
#include "CollisionSystem.hpp"

// Vertex layout for scene geometry
struct SceneVertex {
    glm::vec3 pos;
    glm::vec3 normal;
    glm::vec2 uv;
};

// Per-object uniform buffer data
struct UniformBufferObject {
    alignas(16) glm::mat4 mvpMat;
    alignas(16) glm::mat4 modelMat;
    alignas(16) glm::mat4 normalMat;
};

// Stores and manages all renderable scene objects in the tavern
struct SceneObjects {
    // Imported models
    Model M_Innkeeper;
    Model M_Merchant;
    Model M_SeatedNpc;
    Model M_Orc;

    Model M_Table;
    Model M_Fireplace;
    Model M_BarCounter;
    Model M_BackBar;
    Model M_WallTorch;
    Model M_Door;
    Model M_HangingChain;

    // Procedural room meshes
    Model M_Floor;
    Model M_WallN;
    Model M_WallS;
    Model M_WallE;
    Model M_WallW;
    Model M_Ceiling;

    // Textures
    Texture T_Innkeeper;
    Texture T_Merchant;
    Texture T_SeatedNpc;
    Texture T_Orc;

    Texture T_Table;
    Texture T_Fireplace;
    Texture T_BarCounter;
    Texture T_BackBar;
    Texture T_WallTorch;
    Texture T_Door;
    Texture T_HangingChain;

    Texture T_Floor;
    Texture T_Wall;
    Texture T_Ceiling;

    // Descriptor sets for imported models
    // Each descriptor set connects an object to its texture and per-object uniform buffer
    DescriptorSet DS_Innkeeper;
    DescriptorSet DS_Merchant;
    DescriptorSet DS_SeatedNpc;
    DescriptorSet DS_Orc;

    DescriptorSet DS_Table_A;
    DescriptorSet DS_Table_B;
    DescriptorSet DS_Table_C;

    DescriptorSet DS_Fireplace;
    DescriptorSet DS_BarCounter;
    DescriptorSet DS_BackBar;
    DescriptorSet DS_Door;

    // Descriptor sets for procedural room geometry
    DescriptorSet DS_Floor;
    DescriptorSet DS_WallN;
    DescriptorSet DS_WallS;
    DescriptorSet DS_WallE;
    DescriptorSet DS_WallW;
    DescriptorSet DS_Ceiling;

    // Repeated wall torches
    // One model is reused several times with different transforms
    std::vector<DescriptorSet> DS_WallTorches;
    std::vector<glm::vec3> torchPositions;
    std::vector<float> torchYaws;

    // Repeated hanging chains
    // One model is reused several times with different transforms
    std::vector<DescriptorSet> DS_HangingChains;
    std::vector<glm::vec3> chainPositions;
    std::vector<float> chainRot;
    std::vector<float> chainWallSpin;

    // Loads all imported models, textures, and procedural room meshes
    // Call from localInit()
    void loadAll(BaseProject* bp, VertexDescriptor* VD);

    // Registers collision boundaries for the player/camera
    // This includes room bounds, NPCs, tables, fireplace, and bars
    void registerColliders(CollisionSystem& collisionSystem) const;

    // Returns torch positions
    const std::vector<glm::vec3>& getTorchPositions() const {
        return torchPositions;
    }

    // Creates descriptor sets for all scene objects
    // Call from pipelinesAndDescriptorSetsInit()
    void initDescriptorSets(BaseProject* bp, DescriptorSetLayout* DSL_Object);

    // Releases descriptor sets
    // Call from pipelinesAndDescriptorSetsCleanup()
    void cleanupDescriptorSets();

    // Releases models and textures
    // Call from localCleanup()
    void cleanupAll();

    // Records Vulkan draw commands for all scene objects
    // Call from populateCommandBuffer()
    void drawAll(VkCommandBuffer cb, Pipeline& P, int currentImage);

    // Updates every object's uniform buffer with its current transform
    void updateUBOs(int currentImage,
                    const glm::mat4& proj,
                    const glm::mat4& view,
                    int activeNpcIndex,
                    const glm::vec3& playerPosition);

    // Number of object descriptor sets used by the scene to count thr desriptor pool

    // Count:
    // 11 imported object descriptor sets
    // 4 wall torch instances
    // 8 hanging chain instances
    // 6 room descriptor sets
    // Total = 29
    int count() const { return 29; }
};