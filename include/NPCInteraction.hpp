#pragma once

#include "modules/Starter.hpp"

class NPCInteraction {
private:
    glm::vec3 npcPosition;
    float interactionDistance;
    bool interacted;
    bool eWasPressed;

public:
    NPCInteraction(glm::vec3 position, float distance);

    void update(GLFWwindow* window, const glm::vec3& playerPosition);

    bool hasInteracted() const;
};