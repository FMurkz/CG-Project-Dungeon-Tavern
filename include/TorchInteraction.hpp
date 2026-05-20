#pragma once

#include "modules/Starter.hpp"
#include <vector>
#include <cstddef>

class TorchInteraction {
private:
    std::vector<glm::vec3> torchPositions;
    std::vector<bool> torchIsLit;

    float interactionDistance = 2.5f;
    bool fWasPressed = false;

    int findClosestTorchInRange(const glm::vec3& playerPosition) const;

public:
    void init(const std::vector<glm::vec3>& positions,
              float distance = 2.5f);

    void update(GLFWwindow* window,
                const glm::vec3& playerPosition);

    bool isLit(std::size_t index) const;
    std::size_t count() const;
};