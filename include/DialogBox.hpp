#pragma once

#include "modules/Starter.hpp"
#include <string>

struct DialogVertex {
    glm::vec2 pos;
    glm::vec2 uv;
};

class DialogBox {
private:
    Model         M_DialogBox;
    Texture       T_DialogBox;
    DescriptorSet DS_DialogBox;

public:
    // texturePath is supplied at init time so each DialogBox can show
    // a different image without subclassing or hardcoding.
    void init(BaseProject* bp, VertexDescriptor* VD_UI,
              const std::string& texturePath);

    void initDescriptorSet(BaseProject* bp, DescriptorSetLayout* DSL_UI);

    void cleanup();
    void cleanupDescriptorSet();

    void draw(VkCommandBuffer commandBuffer,
              Pipeline& pipeline,
              int currentImage);
};
