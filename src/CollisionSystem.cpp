#include "CollisionSystem.hpp"
#include <cmath>
#include <algorithm>

namespace {
    //rotates vector by given angle
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

    //player center must stay inside the room, accounting for the player's collision radius.
    roomMinX = minX + playerRadius;
    roomMaxX = maxX - playerRadius;
    roomMinZ = minZ + playerRadius;
    roomMaxZ = maxZ - playerRadius;
}

//creates a rectangular boundary for collisoins
void CollisionSystem::addBoxCollider(const glm::vec2& center,
                                     const glm::vec2& halfSize,
                                     float rotationRadians) {
    boxColliders.push_back({
        center,
        halfSize,
        rotationRadians
    });
}
//creates a circular boundary for collisions
void CollisionSystem::addCircleCollider(const glm::vec2& center,
                                        float radius) {
    circleColliders.push_back({
        center,
        radius
    });
}

//Checks for collision on box space
bool CollisionSystem::collidesWithBoxes(const glm::vec2& playerPosition) const {
    for (const BoxCollider2D& box : boxColliders) {
        //Move player position into the box's local coordinate system
        glm::vec2 relativePosition = playerPosition - box.center;
        glm::vec2 localPosition = rotateVector(relativePosition, -box.rotationRadians);

        //closest point on the rectangle to the player
        glm::vec2 closestPoint = glm::clamp(
            localPosition,
            -box.halfSize,
            box.halfSize
        );
        //vector from player to closest point on box
        glm::vec2 difference = localPosition - closestPoint;
        //collision occurs if distance from box is less than player radius
        if (glm::dot(difference, difference) < playerRadius * playerRadius) {
            return true;
        }
    }

    return false;
}
//Checks for collision on circular space
bool CollisionSystem::collidesWithCircles(const glm::vec2& playerPosition) const {
    for (const CircleCollider2D& circle : circleColliders) {
        //distance from player center to circle center
        glm::vec2 difference = playerPosition - circle.center;
        //Combined radius accounts for both player and circle size
        float combinedRadius = playerRadius + circle.radius;
        //collision occurs if distance from circle is less than player radius
        if (glm::dot(difference, difference) < combinedRadius * combinedRadius) {
            return true;
        }
    }

    return false;
}
//pass position to collider functons
bool CollisionSystem::collidesAt(const glm::vec3& playerPosition) const {
    //only XZ plane is used for collision
    glm::vec2 playerXZ = glm::vec2(playerPosition.x, playerPosition.z);

    return collidesWithBoxes(playerXZ) ||
           collidesWithCircles(playerXZ);
}

glm::vec3 CollisionSystem::movePlayer(const glm::vec3& currentPosition,
                                      const glm::vec3& movementDelta) const {
    glm::vec3 newPosition = currentPosition;

    // small substeps reduce the risk of moving through thin walls in one frame
    float planarDistance = glm::length(glm::vec2(movementDelta.x, movementDelta.z));
    // Minimum step size ensures a reasonable number of iterations
    float safeStepLength = std::max(0.05f, playerRadius * 0.5f);

    int steps = std::max(
        1,
        static_cast<int>(std::ceil(planarDistance / safeStepLength))
    );
    //devide movement into smaller increments for stable collision resolution
    glm::vec3 stepDelta = movementDelta / static_cast<float>(steps);

    for (int i = 0; i < steps; i++) {
        //try X movement separately
        glm::vec3 tryX = newPosition;
        tryX.x += stepDelta.x;

        if (!collidesAt(tryX)) {
            newPosition.x = tryX.x;
        }

        //try Z movement separately
        glm::vec3 tryZ = newPosition;
        tryZ.z += stepDelta.z;

        if (!collidesAt(tryZ)) {
            newPosition.z = tryZ.z;
        }
    }
    //enforce room boundaries after collision resolution
    if (roomBoundsEnabled) {
        newPosition.x = glm::clamp(newPosition.x, roomMinX, roomMaxX);
        newPosition.z = glm::clamp(newPosition.z, roomMinZ, roomMaxZ);
    }

    return newPosition;
}