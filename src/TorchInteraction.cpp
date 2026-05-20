#include "TorchInteraction.hpp"

void TorchInteraction::init(const std::vector<glm::vec3>& positions,
                            float distance) {
    torchPositions = positions;
    interactionDistance = distance;

    // All torches start lit
    torchIsLit.assign(torchPositions.size(), true);
}

int TorchInteraction::findClosestTorchInRange(
    const glm::vec3& playerPosition
) const {
    glm::vec2 playerXZ = glm::vec2(playerPosition.x, playerPosition.z);

    int closestTorchIndex = -1;
    float closestDistance = interactionDistance;

    for (int i = 0; i < static_cast<int>(torchPositions.size()); ++i) {
        glm::vec2 torchXZ = glm::vec2(
            torchPositions[i].x,
            torchPositions[i].z
        );

        float distance = glm::distance(playerXZ, torchXZ);

        if (distance < closestDistance) {
            closestDistance = distance;
            closestTorchIndex = i;
        }
    }

    return closestTorchIndex;
}

void TorchInteraction::update(GLFWwindow* window,
                              const glm::vec3& playerPosition) {
    bool fIsPressed = glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS;

    // Trigger only once per key press
    if (fIsPressed && !fWasPressed) {
        int torchIndex = findClosestTorchInRange(playerPosition);

        if (torchIndex >= 0) {
            torchIsLit[torchIndex] = !torchIsLit[torchIndex];
        }
    }

    fWasPressed = fIsPressed;
}

bool TorchInteraction::isLit(std::size_t index) const {
    if (index >= torchIsLit.size()) {
        return false;
    }

    return torchIsLit[index];
}

std::size_t TorchInteraction::count() const {
    return torchIsLit.size();
}