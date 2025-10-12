#include "Player.h"     // Includes the player class definition
#include "Global.h"     // Shared game data like level boundaries
#include <iostream>     // Lets us print messages for debugging

// This sets the height of the ground in the game world
float groundY = 520;

// Returns the player's hitbox — used to detect collisions with enemies or bushes
Rectangle Player::GetHitbox() const {
    return hitbox;
}

// Returns the sword's hitbox when the player is attacking
Rectangle Player::GetAttackHitbox() const {
    if (!isAttacking) return { 0, 0, 0, 0 }; // No hitbox if not attacking

    float swordWidth = 40;
    float swordHeight = 80;
    float offsetX = facingRight ? hitbox.width : -swordWidth;

    return {
        hitbox.x + offsetX,
        hitbox.y + hitbox.height / 2 - swordHeight / 2,
        swordWidth,
        swordHeight
    };
}

// Keeps a number within a safe range
inline float Clamp(float value, float min, float max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

// Checks if the player is currently attacking
bool Player::IsAttacking() const {
    return isAttacking;
}

// This runs when the player is first created
Player::Player(Vector2 startPos) {
    // Load all the frog textures for different actions
    frogIdle = LoadTexture("Resource Files/idle_right.png");
    frogIdleLeft = LoadTexture("Resource Files/idle_left.png");
    frogJumpRight = LoadTexture("Resource Files/jump_right.png");
    frogJumpLeft = LoadTexture("Resource Files/jump_left.png");
    attackRight = LoadTexture("Resource Files/attack_right.png");
    attackLeft = LoadTexture("Resource Files/attack_left.png");

    // Print error messages if any textures fail to load
    if (attackLeft.id == 0) std::cerr << "Failed to load attack_left.png\n";
    if (attackRight.id == 0) std::cerr << "Failed to load attack_frame.png\n";
    if (frogJumpRight.id == 0) std::cerr << "Failed to load jump_right.png\n";
    if (frogJumpLeft.id == 0) std::cerr << "Failed to load jump_left.png\n";
    if (frogIdle.id == 0) std::cerr << "Failed to load idle_right.png\n";
    if (frogIdleLeft.id == 0) std::cerr << "Failed to load idle_left.png\n";

    // Set the frog's starting position and hitbox
    position = startPos;
    hitbox = { position.x, position.y, width, height };
}

// This updates the frog's movement, jumping, attacking, and reactions
void Player::Update(float dt, const std::vector<Bush>& bushes) {
    previousPosition = position;

    // Apply gravity to pull the frog down
    velocity.y += gravity * dt;
    Vector2 move = { 0, velocity.y * dt };

    // Move left or right based on keyboard input
    if (IsKeyDown(KEY_LEFT)) {
        move.x -= 200 * dt;
        facingRight = false;
    }
    if (IsKeyDown(KEY_RIGHT)) {
        move.x += 200 * dt;
        facingRight = true;
    }

    // Try to move the frog, checking for bush collisions
    TryMove(move, bushes);

    // Jump if the player presses UP and is on the ground
    if (IsKeyPressed(KEY_UP) && isOnGround) {
        velocity.y = jumpForce;
        isOnGround = false;
    }

    // Start attacking if SPACE is pressed
    if (IsKeyPressed(KEY_SPACE) && !isAttacking) {
        isAttacking = true;
        attackTimer = attackDuration;
    }

    // Handle attack timing
    if (isAttacking) {
        attackTimer -= dt;
        if (attackTimer <= 0.0f) {
            isAttacking = false;
        }
    }

    // Handle cooldown after being hit
    if (recentlyHit) {
        hitCooldown -= dt;
        if (hitCooldown <= 0.0f) {
            recentlyHit = false;
        }
    }

    // Handle knockback animation when hit by an enemy
    if (isKnockback) {
        position.x += knockbackVelocity.x * dt;
        position.y += knockbackVelocity.y * dt;
        knockbackTimer -= dt;

        if (knockbackTimer <= 0.0f) {
            isKnockback = false;
            knockbackVelocity = { 0, 0 };
        }

        // Update hitbox and skip normal movement
        hitbox.x = position.x;
        hitbox.y = position.y;
        return;
    }

    // Update vertical position of hitbox
    hitbox.y = position.y;

    // Handle blinking animation (just for fun)
    blinkTimer += dt;

    if (!isBlinking && blinkTimer >= blinkInterval) {
        isBlinking = true;
        blinkTimer = 0.0f;
    }

    if (isBlinking && blinkTimer >= blinkDuration) {
        isBlinking = false;
        blinkTimer = 0.0f;
    }
}

// This draws the frog on screen with the correct texture
void Player::Draw() const {
    DrawRectangleLinesEx(hitbox, 2, RED); // Draws the hitbox for debugging

    const Texture2D* frogTexture = nullptr;

    // Choose the right texture based on what the frog is doing
    if (isAttacking) {
        Rectangle swordHitbox = GetAttackHitbox();
        DrawRectangleRec(swordHitbox, Fade(RED, 0.4f)); // Show sword hitbox
        DrawRectangleLinesEx(swordHitbox, 1, DARKGRAY); // Outline

        frogTexture = facingRight ? &attackRight : &attackLeft;
    }
    else if (!isOnGround) {
        frogTexture = facingRight ? &frogJumpRight : &frogJumpLeft;
    }
    else {
        frogTexture = facingRight ? &frogIdle : &frogIdleLeft;
    }

    // Draw the frog texture if it's loaded
    if (frogTexture && frogTexture->id != 0) {
        Vector2 drawPos = {
            hitbox.x + hitbox.width / 2 - frogTexture->width / 2,
            hitbox.y + hitbox.height - frogTexture->height
        };
        DrawTexture(*frogTexture, (int)drawPos.x, (int)drawPos.y, WHITE);
    }

    // Draw blinking eyes if blinking is active
    if (isBlinking) {
        float eyeY = hitbox.y + 23;
        float eyeWidth = 4;
        float eyeHeight = 2;

        float leftEyeX = hitbox.x + hitbox.width / 2 - 12;
        float rightEyeX = hitbox.x + hitbox.width / 2 + 8;

        DrawRectangle(leftEyeX, eyeY, eyeWidth, eyeHeight, PINK);
        DrawRectangle(rightEyeX, eyeY, eyeWidth, eyeHeight, PINK);
    }
}

// Starts the knockback effect when the frog gets hit
void Player::StartKnockback(Vector2 velocity) {
    isKnockback = true;
    knockbackVelocity = velocity;
    knockbackTimer = knockbackDuration;
}

// Cleans up textures when the game ends
Player::~Player() {
    UnloadTexture(frogIdle);
    UnloadTexture(frogIdleLeft);
    UnloadTexture(frogJumpRight);
    UnloadTexture(frogJumpLeft);
}

// Returns the frog's current position (used by camera and other systems)
Vector2 Player::GetPosition() const {
    return position;
}

// Handles movement and collision with bushes and ground
void Player::TryMove(Vector2 delta, const std::vector<Bush>& bushes) {
    Rectangle proposed = { position.x + delta.x, position.y + delta.y, hitbox.width, hitbox.height };

    // Keep the frog inside the level boundaries
    if (proposed.x < levelBounds.x) proposed.x = levelBounds.x;
    if (proposed.x + proposed.width > levelBounds.x + levelBounds.width)
        proposed.x = levelBounds.x + levelBounds.width - proposed.width;

    if (proposed.y < levelBounds.y) proposed.y = levelBounds.y;
    if (proposed.y + proposed.height > levelBounds.y + levelBounds.height)
        proposed.y = levelBounds.y + levelBounds.height - proposed.height;

    // Handle horizontal movement and bush collisions
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