#include "NPCInteraction.hpp"

NPCInteraction::NPCInteraction(const NPCInteractionDef& def)
    : def(def) {}

void NPCInteraction::update(GLFWwindow* window, const glm::vec3& playerPosition) {
    glm::vec2 playerXZ = glm::vec2(playerPosition.x, playerPosition.z);
    glm::vec2 npcXZ    = glm::vec2(def.position.x,   def.position.z);

    bool isNear   = glm::distance(playerXZ, npcXZ) < def.interactionDistance;
    bool ePressed = glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS;

    // Toggle dialog open/closed on a fresh E press while nearby
    if (isNear && ePressed && !eWasPressed)
        interacted = !interacted;

    // Auto-close when the player walks away
    if (!isNear)
        interacted = false;

    eWasPressed = ePressed;
}

bool NPCInteraction::hasInteracted() const {
    return interacted;
}

bool NPCInteraction::isPlayerNear() const {
    return interacted;
}

const std::string& NPCInteraction::getDialogTexturePath() const {
    return def.dialogTexturePath;
}
