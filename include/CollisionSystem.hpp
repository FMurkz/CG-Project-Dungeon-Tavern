#pragma once

#include "modules/Starter.hpp"
#include <vector>

struct BoxCollider2D {
    glm::vec2 center;
    glm::vec2 halfSize;
    float rotationRadians;
};

struct CircleCollider2D {
    glm::vec2 center;
    float radius;
};

class CollisionSystem {
private:
    std::vector<BoxCollider2D> boxColliders;
    std::vector<CircleCollider2D> circleColliders;

    float playerRadius = 0.35f;

    bool roomBoundsEnabled = false;
    float roomMinX = 0.0f;
    float roomMaxX = 0.0f;
    float roomMinZ = 0.0f;
    float roomMaxZ = 0.0f;

    bool collidesWithBoxes(const glm::vec2& playerPosition) const;
    bool collidesWithCircles(const glm::vec2& playerPosition) const;

public:
    void clear();

    void setPlayerRadius(float radius);

    void setRoomBounds(float minX,
                   float maxX,
                   float minZ,
                   float maxZ);

    void addBoxCollider(const glm::vec2& center,
                        const glm::vec2& halfSize,
                        float rotationRadians = 0.0f);

    void addCircleCollider(const glm::vec2& center,
                           float radius);

    bool collidesAt(const glm::vec3& playerPosition) const;

    glm::vec3 movePlayer(const glm::vec3& currentPosition,
                         const glm::vec3& movementDelta) const;
};