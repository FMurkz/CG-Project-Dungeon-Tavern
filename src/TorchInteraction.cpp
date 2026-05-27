#include "TorchInteraction.hpp"

void TorchInteraction::init(const std::vector<glm::vec3>& positions,
                            float distance) {
    torchPositions = positions;
    interactionDistance = distance;

    //start with all torches start lit
    torchIsLit.assign(torchPositions.size(), true);
}
//find closest torch within interaction range
int TorchInteraction::findClosestTorchInRange(
    const glm::vec3& playerPosition
) const {
    //ignore height (work in XZ plane)
    glm::vec2 playerXZ = glm::vec2(playerPosition.x, playerPosition.z);

    int closestTorchIndex = -1;
    float closestDistance = interactionDistance;
    //check distance for each torch
    for (int i = 0; i < static_cast<int>(torchPositions.size()); ++i) {
        glm::vec2 torchXZ = glm::vec2(
            torchPositions[i].x,
            torchPositions[i].z
        );

        float distance = glm::distance(playerXZ, torchXZ);
        //keep closest torch within range
        if (distance < closestDistance) {
            closestDistance = distance;
            closestTorchIndex = i;
        }
    }

    return closestTorchIndex;
}
//Update torch interaction each frame
void TorchInteraction::update(GLFWwindow* window,
                              const glm::vec3& playerPosition) {
    //chack if F is pressed
    bool fIsPressed = glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS;

    //trigger only on new key press
    if (fIsPressed && !fWasPressed) {
        int torchIndex = findClosestTorchInRange(playerPosition);
        //Toggle torch state
        if (torchIndex >= 0) {
            torchIsLit[torchIndex] = !torchIsLit[torchIndex];
        }
    }
    //store key state
    fWasPressed = fIsPressed;
}

// Check if a torch is lit
bool TorchInteraction::isLit(std::size_t index) const {
    if (index >= torchIsLit.size()) {
        return false;
    }

    return torchIsLit[index];
}

std::size_t TorchInteraction::count() const {
    return torchIsLit.size();
}