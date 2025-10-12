#include "Player.h"
#include "Global.h"
#include <iostream>

float groundY = 520;

Rectangle Player::GetHitbox() const {
    return hitbox;
}

Rectangle Player::GetAttackHitbox() const {
    if (!isAttacking) return { 0, 0, 0, 0 };

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

inline float Clamp(float value, float min, float max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

bool Player::IsAttacking() const {
    return isAttacking;
}

// Constructor: initializes the player's position
Player::Player(Vector2 startPos) {
    frogIdle = LoadTexture("Resource Files/idle_right.png");
    frogIdleLeft = LoadTexture("Resource Files/idle_left.png");
    frogJumpRight = LoadTexture("Resource Files/jump_right.png");
    frogJumpLeft = LoadTexture("Resource Files/jump_left.png");
    attackRight = LoadTexture("Resource Files/attack_right.png");
    attackLeft = LoadTexture("Resource Files/attack_left.png");

    if (attackLeft.id == 0) std::cerr << "Failed to load attack_left.png\n";
    if (attackRight.id == 0) std::cerr << "Failed to load attack_frame.png\n";
    if (frogJumpRight.id == 0) std::cerr << "Failed to load jump_right.png\n";
    if (frogJumpLeft.id == 0) std::cerr << "Failed to load jump_left.png\n";
    if (frogIdle.id == 0) std::cerr << "Failed to load idle_right.png\n";
    if (frogIdleLeft.id == 0) std::cerr << "Failed to load idle_left.png\n";

    position = startPos;
    hitbox = { position.x, position.y, width, height };
}

// Updates player movement based on keyboard input
void Player::Update(float dt, const std::vector<Bush>& bushes) {
    previousPosition = position;
    velocity.y += gravity * dt; // apply gravity
    Vector2 move = { 0, velocity.y * dt };

	//Left and right input
    if (IsKeyDown(KEY_LEFT)) {
        move.x -= 200 * dt;
        facingRight = false;
    }
    if (IsKeyDown(KEY_RIGHT)) {
        move.x += 200 * dt;
        facingRight = true;
    }


    TryMove(move, bushes);

    // Jump input
    if (IsKeyPressed(KEY_UP) && isOnGround) {
        velocity.y = jumpForce;
        isOnGround = false;
    }

	// Attack input
    if (IsKeyPressed(KEY_SPACE) && !isAttacking) {
        isAttacking = true;
        attackTimer = attackDuration;
    }
    if (isAttacking) {
        attackTimer -= dt;
        if (attackTimer <= 0.0f) {
            isAttacking = false;
        }
    }

    if (recentlyHit) {
        hitCooldown -= dt;
        if (hitCooldown <= 0.0f) {
            recentlyHit = false;
        }
    }

    if (isKnockback) {
        position.x += knockbackVelocity.x * dt;
        position.y += knockbackVelocity.y * dt;
        knockbackTimer -= dt;

        if (knockbackTimer <= 0.0f) {
            isKnockback = false;
            knockbackVelocity = { 0, 0 };
        }

        // Skip normal movement while bouncing
        hitbox.x = position.x;
        hitbox.y = position.y;
        return;
    }

    hitbox.y = position.y;

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

// Draws the player as a blue rectangle
void Player::Draw() const {
    DrawRectangleLinesEx(hitbox, 2, RED); // shows hitbox

    const Texture2D* frogTexture = nullptr;

    if (isAttacking) {
        Rectangle swordHitbox = GetAttackHitbox();
        DrawRectangleRec(swordHitbox, Fade(RED, 0.4f)); // semi-transparent red
        DrawRectangleLinesEx(swordHitbox, 1, DARKGRAY); // outline

        frogTexture = facingRight ? &attackRight : &attackLeft;
    }
    else if (!isOnGround) {
        frogTexture = facingRight ? &frogJumpRight : &frogJumpLeft;
    }
    else {
        frogTexture = facingRight ? &frogIdle : &frogIdleLeft;
    }

    if (frogTexture && frogTexture->id != 0) {
        Vector2 drawPos = {
            hitbox.x + hitbox.width / 2 - frogTexture->width / 2,
            hitbox.y + hitbox.height - frogTexture->height
        };
        DrawTexture(*frogTexture, (int)drawPos.x, (int)drawPos.y, WHITE);
    }

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

void Player::StartKnockback(Vector2 velocity) {
    isKnockback = true;
    knockbackVelocity = velocity;
    knockbackTimer = knockbackDuration;
}

Player::~Player() {
    UnloadTexture(frogIdle);
    UnloadTexture(frogIdleLeft);
    UnloadTexture(frogJumpRight);
    UnloadTexture(frogJumpLeft);
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