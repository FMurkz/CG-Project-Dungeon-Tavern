#include "DialogBox.hpp"

void DialogBox::init(BaseProject* bp, VertexDescriptor* VD_UI,
                     const std::string& texturePath) {
    // Rectangle in normalized device coordinates.
    // x: -1 = left edge,   1 = right edge
    // y: -1 = bottom edge, 1 = top edge
    std::vector<DialogVertex> dialogVerts = {
        {{-0.95f,  0.45f}, {0.0f, 0.0f}},
        {{ 0.10f,  0.45f}, {1.0f, 0.0f}},
        {{ 0.10f,  0.92f}, {1.0f, 1.0f}},
        {{-0.95f,  0.92f}, {0.0f, 1.0f}},
    };

    M_DialogBox.vertices = std::vector<unsigned char>(
        reinterpret_cast<unsigned char*>(dialogVerts.data()),
        reinterpret_cast<unsigned char*>(dialogVerts.data())
            + dialogVerts.size() * sizeof(DialogVertex)
    );
    M_DialogBox.indices = {0, 1, 2,  0, 2, 3};
    M_DialogBox.initMesh(bp, VD_UI);

    // Use the path provided by the caller instead of a hardcoded string
    T_DialogBox.init(bp, texturePath);
}

void DialogBox::initDescriptorSet(BaseProject* bp,
                                   DescriptorSetLayout* DSL_UI) {
    DS_DialogBox.init(
        bp,
        DSL_UI,
        {T_DialogBox.getViewAndSampler()}
    );
}

void DialogBox::cleanup() {
    M_DialogBox.cleanup();
    T_DialogBox.cleanup();
}

void DialogBox::cleanupDescriptorSet() {
    DS_DialogBox.cleanup();
}

void DialogBox::draw(VkCommandBuffer commandBuffer,
                     Pipeline& pipeline,
                     int currentImage) {
    DS_DialogBox.bind(commandBuffer, pipeline, 0, currentImage);
    M_DialogBox.bind(commandBuffer);
    vkCmdDrawIndexed(
        commandBuffer,
        static_cast<uint32_t>(M_DialogBox.indices.size()),
        1, 0, 0, 0
    );
}
