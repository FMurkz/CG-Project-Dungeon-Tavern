#include "NPCInteraction.hpp"

NPCInteraction::NPCInteraction(glm::vec3 position, float distance)
    : npcPosition(position),
      interactionDistance(distance),
      interacted(false),
      eWasPressed(false),
      playerIsNearNpc(false){}

void NPCInteraction::update(GLFWwindow* window, const glm::vec3& playerPosition) {
    glm::vec2 playerXZ = glm::vec2(playerPosition.x, playerPosition.z);
    glm::vec2 npcXZ    = glm::vec2(npcPosition.x, npcPosition.z);

    float distanceToNpc = glm::distance(playerXZ, npcXZ);
    playerIsNearNpc = distanceToNpc < interactionDistance;

    bool eIsPressed = glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS;

    if (playerIsNearNpc && eIsPressed && !eWasPressed) {
        interacted = !interacted;
    }

    eWasPressed = eIsPressed;
}

bool NPCInteraction::hasInteracted() const {
    return interacted;
}

bool NPCInteraction::isPlayerNearNpc() const {
    return playerIsNearNpc;
}