#pragma once

#include "modules/Starter.hpp"
#include <string>

// Describes one interactable NPC in the world.
// Fill one of these per NPC and pass it to NPCInteraction's constructor.
struct NPCInteractionDef {
    glm::vec3   position;
    float       interactionDistance;
    std::string dialogTexturePath;   // e.g. "assets/ui/innkeeper_dialog.png"
};

class NPCInteraction {
private:
    NPCInteractionDef def;
    bool interacted  = false;
    bool eWasPressed = false;

public:
    explicit NPCInteraction(const NPCInteractionDef& def);

    // Call every frame with the current window and player world-position.
    void update(GLFWwindow* window, const glm::vec3& playerPosition);

    bool hasInteracted() const;
    bool isPlayerNear()  const;

    const std::string& getDialogTexturePath() const;
};
