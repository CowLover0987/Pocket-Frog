#include "Player.h"
#include "Global.h"
#include <iostream>

float groundY = 520;
int maxFrames = 6;

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
    frogIdle = LoadTexture("Resource Files/idle_right.png");

    if (frogIdle.id == 0) {
        std::cerr << "Failed to load: Resource Files/frog_idle.png\n";
    }

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
    bool isMoving = false;

    /*if (IsKeyDown(KEY_LEFT)) {
        velocity.x = -200*dt;
        currentStrip = &walkLeftStrip;
        isMoving = true;
    }
    if (IsKeyDown(KEY_RIGHT)) {
        velocity.x = 200*dt;
        currentStrip = &walkRightStrip;
        isMoving = true;
    }*/

    if (IsKeyDown(KEY_LEFT)) { move.x -= 200 * dt; }
    if (IsKeyDown(KEY_RIGHT)) { move.x += 200 * dt; }


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
    DrawRectangleLinesEx(hitbox, 2, RED); // shows hitbox
    if (frogIdle.id != 0) {
        Vector2 drawPos = {
            hitbox.x + hitbox.width / 2 - frogIdle.width / 2,
            hitbox.y + hitbox.height - frogIdle.height
        };
        DrawTexture(frogIdle, drawPos.x, drawPos.y, WHITE);
    }
}

Player::~Player() {
    UnloadTexture(frogIdle);
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