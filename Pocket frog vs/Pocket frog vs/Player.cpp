#include "Player.h"
#include "Global.h"
#include <iostream>

float groundY = 520;
int maxFrames = 4;

// Declare textures but don't load them at global scope
Texture2D walkRightStrip = { 0 };
Texture2D walkLeftStrip = { 0 };
Texture2D* currentStrip = &walkRightStrip; // default facing right

Rectangle frameRec = { 0, 0, 0, 0 };

Rectangle Player::GetHitbox() const {
    return hitbox;
}

inline float Clamp(float value, float min, float max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

// Helper to load textures once (call after InitWindow)
static void EnsurePlayerResourcesLoaded() {
    static bool loaded = false;
    if (loaded) return;

    walkRightStrip = LoadTexture("Resource Files/right_walking_animation.png");
    walkLeftStrip = LoadTexture("Resource Files/left_walking_animation.png");

    if (walkRightStrip.id == 0) {
        std::cerr << "Failed to load: Resource Files/right_walking_animation.png\n";
    }
    if (walkLeftStrip.id == 0) {
        std::cerr << "Failed to load: Resource Files/left_walking_animation.png\n";
    }

    // Protect against division by zero if load failed
    if (walkRightStrip.width > 0 && walkRightStrip.height > 0) {
        // Fix for E2361: invalid narrowing conversion from "int" to "float"
        // Change walkRightStrip.width / maxFrames and walkRightStrip.height to float
        frameRec = { 0, 0, static_cast<float>(walkRightStrip.width) / static_cast<float>(maxFrames), static_cast<float>(walkRightStrip.height) };
    } else {
        frameRec = { 0, 0, 32, 32 }; // fallback
    }

    loaded = true;
}

// Constructor: initializes the player's position
Player::Player(Vector2 startPos) {
    // Ensure textures are loaded after InitWindow (main constructs player after InitWindow)
    EnsurePlayerResourcesLoaded();

    position = startPos;
    hitbox = { position.x, position.y, width, height };
}

// Updates player movement based on keyboard input
void Player::Update(float dt, const std::vector<Bush>& bushes) {
    previousPosition = position;
    velocity.y += gravity * dt; // apply gravity
    Vector2 move = { 0, velocity.y * dt };
    if (IsKeyDown(KEY_LEFT)) {
        move.x -= 200 * dt;
        currentStrip = &walkLeftStrip;
    }
    if (IsKeyDown(KEY_RIGHT)) {
        move.x += 200 * dt;
        currentStrip = &walkRightStrip;
    }

    TryMove(move, bushes);

    // Jump input
    if (IsKeyPressed(KEY_SPACE) && isOnGround) {
        velocity.y = jumpForce;
        isOnGround = false;
    }

    hitbox.y = position.y;

    frameTime += dt;
    if (frameTime >= frameSpeed) {
        frameTime = 0;
        currentFrame++;
        if (currentFrame >= maxFrames) currentFrame = 0;
        frameRec.x = currentFrame * frameRec.width;
    }
}

// Draws the player as a blue rectangle
void Player::Draw() const {
    DrawRectangleV(position, Vector2{ width, height }, BLUE);
    if (currentStrip && currentStrip->id != 0) {
        DrawTextureRec(*currentStrip, frameRec, position, WHITE);
    }
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