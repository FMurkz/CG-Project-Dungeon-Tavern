#include "CollisionSystem.hpp"
#include <cmath>
#include <algorithm>

namespace {
    glm::vec2 rotateVector(const glm::vec2& v, float angle) {
        float c = std::cos(angle);
        float s = std::sin(angle);

        return glm::vec2(
            c * v.x - s * v.y,
            s * v.x + c * v.y
        );
    }
}

void CollisionSystem::clear() {
    boxColliders.clear();
    circleColliders.clear();
}

void CollisionSystem::setPlayerRadius(float radius) {
    playerRadius = radius;
}

void CollisionSystem::setRoomBounds(float minX,
                                    float maxX,
                                    float minZ,
                                    float maxZ) {
    roomBoundsEnabled = true;

    // The player's center must stay inside the room, accounting for the player's collision radius.
    roomMinX = minX + playerRadius;
    roomMaxX = maxX - playerRadius;
    roomMinZ = minZ + playerRadius;
    roomMaxZ = maxZ - playerRadius;
}

void CollisionSystem::addBoxCollider(const glm::vec2& center,
                                     const glm::vec2& halfSize,
                                     float rotationRadians) {
    boxColliders.push_back({
        center,
        halfSize,
        rotationRadians
    });
}

void CollisionSystem::addCircleCollider(const glm::vec2& center,
                                        float radius) {
    circleColliders.push_back({
        center,
        radius
    });
}

bool CollisionSystem::collidesWithBoxes(const glm::vec2& playerPosition) const {
    for (const BoxCollider2D& box : boxColliders) {
        // Move player position into the box's local coordinate system
        glm::vec2 relativePosition = playerPosition - box.center;
        glm::vec2 localPosition = rotateVector(relativePosition, -box.rotationRadians);

        // Closest point on the rectangle to the player
        glm::vec2 closestPoint = glm::clamp(
            localPosition,
            -box.halfSize,
            box.halfSize
        );

        glm::vec2 difference = localPosition - closestPoint;

        if (glm::dot(difference, difference) < playerRadius * playerRadius) {
            return true;
        }
    }

    return false;
}

bool CollisionSystem::collidesWithCircles(const glm::vec2& playerPosition) const {
    for (const CircleCollider2D& circle : circleColliders) {
        glm::vec2 difference = playerPosition - circle.center;
        float combinedRadius = playerRadius + circle.radius;

        if (glm::dot(difference, difference) < combinedRadius * combinedRadius) {
            return true;
        }
    }

    return false;
}

bool CollisionSystem::collidesAt(const glm::vec3& playerPosition) const {
    glm::vec2 playerXZ = glm::vec2(playerPosition.x, playerPosition.z);

    return collidesWithBoxes(playerXZ) ||
           collidesWithCircles(playerXZ);
}

glm::vec3 CollisionSystem::movePlayer(const glm::vec3& currentPosition,
                                      const glm::vec3& movementDelta) const {
    glm::vec3 newPosition = currentPosition;

    // Small substeps reduce the risk of moving through thin walls in one frame
    float planarDistance = glm::length(glm::vec2(movementDelta.x, movementDelta.z));
    float safeStepLength = std::max(0.05f, playerRadius * 0.5f);

    int steps = std::max(
        1,
        static_cast<int>(std::ceil(planarDistance / safeStepLength))
    );

    glm::vec3 stepDelta = movementDelta / static_cast<float>(steps);

    for (int i = 0; i < steps; i++) {
        // Try X movement separately
        glm::vec3 tryX = newPosition;
        tryX.x += stepDelta.x;

        if (!collidesAt(tryX)) {
            newPosition.x = tryX.x;
        }

        // Try Z movement separately
        glm::vec3 tryZ = newPosition;
        tryZ.z += stepDelta.z;

        if (!collidesAt(tryZ)) {
            newPosition.z = tryZ.z;
        }
    }

    if (roomBoundsEnabled) {
        newPosition.x = glm::clamp(newPosition.x, roomMinX, roomMaxX);
        newPosition.z = glm::clamp(newPosition.z, roomMinZ, roomMaxZ);
    }

    return newPosition;
}