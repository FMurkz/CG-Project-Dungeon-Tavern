#include "modules/Starter.hpp"

class DungeonTavern : public BaseProject {
protected:
    void setWindowParameters() {
        windowWidth = 800;
        windowHeight = 600;
        windowTitle = "Dungeon Tavern";
        windowResizable = GLFW_TRUE;
        initialBackgroundColor = {0.05f, 0.05f, 0.1f, 1.0f};
    }

    void onWindowResize(int w, int h) {}
    void localInit() {}
    void pipelinesAndDescriptorSetsInit() {}
    void pipelinesAndDescriptorSetsCleanup() {}
    void localCleanup() {}
    void populateCommandBuffer(VkCommandBuffer commandBuffer, int currentImage) {}
    void updateUniformBuffer(uint32_t currentImage) {}
};

int main() {
    DungeonTavern app;
    try {
        app.run();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}