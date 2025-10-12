#pragma once
#include "raylib.h"
#include <vector> // Add this at the top of the file, before using std::vector
#include "bush.h"

// Defines a simple player class for side-scrolling movement
class Player {
public:
    // Constructor: sets the starting position of the player
    Player(Vector2 startPos);
    ~Player();
    // Updates player logic (movement, etc.)
    void Update(float dt, const std::vector<Bush>& bushes);

    // Draws the player to the screen
    void Draw() const;

    // Returns the current position of the player
    Vector2 GetPosition() const;

    //Checks for collisions
    void TryMove(Vector2 delta, const std::vector<Bush>& bushes);

    Vector2 previousPosition;
    Rectangle GetHitbox() const;
    Rectangle GetAttackHitbox() const;
    void StartKnockback(Vector2 velocity);

    bool IsAttacking() const;
    int health = 5;
    bool recentlyHit = false;
    float hitCooldown = 0.0f;
    const float hitCooldownDuration = 1.0f; // seconds

    bool isKnockback = false;
    Vector2 knockbackVelocity = { 0, 0 };
    float knockbackTimer = 0.0f;
    const float knockbackDuration = 0.2f; // seconds

private:
    Vector2 position;     // Player's position in the world
    Rectangle hitbox;    // Player's collision rectangle
    float speed = 200.0f; // Movement speed in pixels per second
    float width = 50;     // Width of the player rectangle
    float height = 80;    // Height of the player rectangle
    Vector2 velocity = { 0, 0 };
    float gravity = 800.0f;       // pixels per second²
    float jumpForce = -500.0f;    // upward velocity
    bool isOnGround = false;
    bool facingRight = true;

    bool isBlinking = false;
    float blinkTimer = 0.0f;
    float blinkInterval = 3.0f; // seconds between blinks
    float blinkDuration = 0.2f; // how long the blink lasts
    
    bool isAttacking = false;
    float attackTimer = 0.0f;
    const float attackDuration = 0.2f; // seconds
    

    Texture2D frogIdle = { 0 };
    Texture2D frogIdleLeft = { 0 };
    Texture2D frogJumpRight = { 0 };
    Texture2D frogJumpLeft = { 0 };
    Texture2D attackRight = { 0 };
    Texture2D attackLeft = { 0 };
};