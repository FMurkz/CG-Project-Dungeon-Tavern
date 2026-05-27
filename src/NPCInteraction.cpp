#include "NPCInteraction.hpp"

NPCInteraction::NPCInteraction(const NPCInteractionDef& def)
    : def(def) {}
//update interaction each frame
void NPCInteraction::update(GLFWwindow* window, const glm::vec3& playerPosition) {
    //ignore y space
    glm::vec2 playerXZ = glm::vec2(playerPosition.x, playerPosition.z);
    glm::vec2 npcXZ    = glm::vec2(def.position.x,   def.position.z);

    //chack if player is near nps
    playerIsNear = glm::distance(playerXZ, npcXZ) < def.interactionDistance;
    bool ePressed = glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS;

    //Toggle dialog open/closed if e is pressed
    if (playerIsNear && ePressed && !eWasPressed)
        interacted = !interacted;

    //auto-close when the player walks away
    if (!playerIsNear)
        interacted = false;

    eWasPressed = ePressed;
}

bool NPCInteraction::hasInteracted() const {
    return interacted;
}

bool NPCInteraction::isPlayerNear() const {
    return playerIsNear;
}

const std::string& NPCInteraction::getDialogTexturePath() const {
    return def.dialogTexturePath;
}
