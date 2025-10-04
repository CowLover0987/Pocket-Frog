#include "Enemy.h"
#include <cmath>
#include "Global.h"


inline float Clamp(float value, float min, float max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

Enemy::Enemy(Vector2 startPos) {
    position = startPos;
}

void Enemy::Update(float dt, Vector2 playerPos, const std::vector<Bush>& bushes) {
    Vector2 toPlayer = { playerPos.x - position.x, playerPos.y - position.y };
    float distance = std::sqrt(toPlayer.x * toPlayer.x + toPlayer.y * toPlayer.y);

    if (distance < detectionRadius) {
        // Normalize direction (x-axis only)
        float length = std::sqrt(toPlayer.x * toPlayer.x);
        if (length != 0) {
            velocity.x = (toPlayer.x / length) * chaseSpeed;
        }
        velocity.y = 0; // no vertical movement
    }
    else {
        // Wander slowly left/right
        velocity.x = std::sin(GetTime()) * speed;
        velocity.y = 0; // no vertical movement
    }

    Vector2 move = { velocity.x * dt, 0 }; // only x-axis
    TryMove(move, bushes);
}

void Enemy::Draw() const {
    DrawRectangleV(position, Vector2{ width, height }, ORANGE);
}

Rectangle Enemy::GetCollider() const {
    return { position.x, position.y, width, height };
}

void Enemy::TryMove(Vector2 delta, const std::vector<Bush>& bushes) {
    Rectangle proposed = { position.x + delta.x, position.y + delta.y, width, height };

    // Clamp to level bounds
    proposed.x = Clamp(proposed.x, levelBounds.x, levelBounds.x + levelBounds.width - width);
    proposed.y = Clamp(proposed.y, levelBounds.y, levelBounds.y + levelBounds.height - height);

    // Check bush collisions
    for (const Bush& bush : bushes) {
        if (CheckCollisionRecs(proposed, bush.GetCollider())) {
            return; // Block movement
        }
    }

    // Apply movement
    position.x = proposed.x;
    position.y = proposed.y;
}