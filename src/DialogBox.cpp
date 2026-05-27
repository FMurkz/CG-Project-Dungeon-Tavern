#include "DialogBox.hpp"

void DialogBox::init(BaseProject* bp, VertexDescriptor* VD_UI,
                     const std::string& texturePath) {
    //define dialog box in Normalized Device Coordinates (NDC)
    std::vector<DialogVertex> dialogVerts = {
        {{-0.95f,  0.45f}, {0.0f, 0.0f}},
        {{ 0.10f,  0.45f}, {1.0f, 0.0f}},
        {{ 0.10f,  0.92f}, {1.0f, 1.0f}},
        {{-0.95f,  0.92f}, {0.0f, 1.0f}},
    };
    //convert vertices to raw bytes for GPU
    M_DialogBox.vertices = std::vector<unsigned char>(
        reinterpret_cast<unsigned char*>(dialogVerts.data()),
        reinterpret_cast<unsigned char*>(dialogVerts.data())
            + dialogVerts.size() * sizeof(DialogVertex)
    );
    // two triangles combined to one rectagle
    M_DialogBox.indices = {0, 1, 2,  0, 2, 3};

    M_DialogBox.initMesh(bp, VD_UI);
    T_DialogBox.init(bp, texturePath);
}

void DialogBox::initDescriptorSet(BaseProject* bp,
                                   DescriptorSetLayout* DSL_UI) {
    // Bind texture to shader
    DS_DialogBox.init(bp, DSL_UI,{T_DialogBox.getViewAndSampler()});
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
    // draw box
    vkCmdDrawIndexed(
        commandBuffer,
        static_cast<uint32_t>(M_DialogBox.indices.size()),
        1, 0, 0, 0
    );
}
