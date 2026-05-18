#pragma once

#include "modules/Starter.hpp"

struct DialogVertex {
    glm::vec2 pos;
    glm::vec2 uv;
};

class DialogBox {
private:
    Model M_DialogBox;

    // The PNG image shown in the dialogue box
    Texture T_DialogBox;

    // Descriptor set that exposes the PNG texture to the UI fragment shader
    DescriptorSet DS_DialogBox;

public:
    void init(BaseProject* bp, VertexDescriptor* VD_UI);

    void initDescriptorSet(BaseProject* bp,
                           DescriptorSetLayout* DSL_UI);

    void cleanup();
    void cleanupDescriptorSet();

    void draw(VkCommandBuffer commandBuffer,
              Pipeline& pipeline,
              int currentImage);
};