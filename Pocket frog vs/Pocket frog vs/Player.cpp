#include "Player.h"
#include "Global.h"

float groundY = 520;

Rectangle Player::GetHitbox() const {
    return hitbox;
}

inline float Clamp(float value, float min, float max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

// Constructor: initializes the player's position
Player::Player(Vector2 startPos) {
    position = startPos;
    hitbox = { position.x, position.y, width, height };
}

// Updates player movement based on keyboard input
void Player::Update(float dt, const std::vector<Bush>& bushes) {
    previousPosition = position;
    velocity.y += gravity * dt; // apply gravity
    Vector2 move = { 0, velocity.y * dt };
    if (IsKeyDown(KEY_LEFT)) move.x -= 200 * dt;
    if (IsKeyDown(KEY_RIGHT)) move.x += 200 * dt;

    TryMove(move, bushes);

// Jump input
    if (IsKeyPressed(KEY_SPACE) && isOnGround) {
        velocity.y = jumpForce;
        isOnGround = false;
    }

    hitbox.y = position.y;
}

// Draws the player as a blue rectangle
void Player::Draw() const {
    DrawRectangleV(position, Vector2{ width, height }, BLUE);
}

// Returns the player's current position (used by camera or other systems)
Vector2 Player::GetPosition() const {
    return position;
}

void Player::TryMove(Vector2 delta, const std::vector<Bush>& bushes) {
    Rectangle proposed = { position.x + delta.x, position.y + delta.y, hitbox.width, hitbox.height };


    // Clamp to level bounds
    if (proposed.x < levelBounds.x) proposed.x = levelBounds.x;
    if (proposed.x + proposed.width > levelBounds.x + levelBounds.width)
        proposed.x = levelBounds.x + levelBounds.width - proposed.width;

    if (proposed.y < levelBounds.y) proposed.y = levelBounds.y;
    if (proposed.y + proposed.height > levelBounds.y + levelBounds.height)
        proposed.y = levelBounds.y + levelBounds.height - proposed.height;

    // Horizontal movement
    Rectangle proposedX = { position.x + delta.x, position.y, hitbox.width, hitbox.height };
    proposedX.x = Clamp(proposedX.x, levelBounds.x, levelBounds.x + levelBounds.width - hitbox.width);

    bool blockedX = false;
    for (const Bush& bush : bushes) {
        Rectangle bushCollider = bush.GetCollider();
        bool verticallyAligned = proposedX.y + proposedX.height > bushCollider.y &&
            proposedX.y < bushCollider.y + bushCollider.height;

        if (verticallyAligned && CheckCollisionRecs(proposedX, bushCollider)) {
            blockedX = true;
            break;
        }
    }

    if (!blockedX) {
        position.x = proposedX.x;
    }

    // Vertical movement
    Rectangle proposedY = { position.x, position.y + delta.y, hitbox.width, hitbox.height };
    proposedY.y = Clamp(proposedY.y, levelBounds.y, levelBounds.y + levelBounds.height - hitbox.height);

    bool landed = false;
    bool blockedY = false;

    for (const Bush& bush : bushes) {
        Rectangle bushCollider = bush.GetCollider();
        if (CheckCollisionRecs(proposedY, bushCollider)) {
            bool landedOnBush = previousPosition.y + hitbox.height <= bushCollider.y + 2 &&
                velocity.y > 0;

            if (landedOnBush) {
                position.y = bushCollider.y - hitbox.height;
                velocity.y = 0;
                isOnGround = true;
                landed = true;
                break;
            }
            else {
                blockedY = true;
                velocity.y = 0;
                break;
            }
        }
    }

    if (!landed && !blockedY) {
        position.y = proposedY.y;
        isOnGround = false;
    }

    // Ground snap
    float frogFeet = position.y + hitbox.height;
    if (frogFeet >= groundY - 1) {
        position.y = groundY - hitbox.height;
        velocity.y = 0;
        isOnGround = true;
    }

    // Update hitbox
    hitbox.x = position.x;
    hitbox.y = position.y;
}