#pragma once // Makes sure this file is only included once during compilation

#include "raylib.h"     // Gives access to drawing tools and game types like Vector2 and Texture2D
#include <vector>       // Lets us use lists of bushes
#include "bush.h"       // So the player can check for bush collisions

// This is the blueprint for your frog player character.
// It defines how the frog moves, jumps, attacks, and reacts to enemies.
class Player {
public:
    // This creates the player at a starting position in the world
    Player(Vector2 startPos);

    // This cleans up the player when the game ends
    ~Player();

    // This updates the player's behavior every frame (movement, jumping, attacking, etc.)
    void Update(float dt, const std::vector<Bush>& bushes);

    // This draws the player on the screen
    void Draw() const;

    // This returns the player's current position
    Vector2 GetPosition() const;

    // This handles movement and checks for bush collisions
    void TryMove(Vector2 delta, const std::vector<Bush>& bushes);

    // Stores the player's previous position (used for movement logic)
    Vector2 previousPosition;

    // Returns the player's hitbox — used for detecting enemy contact
    Rectangle GetHitbox() const;

    // Returns the sword's hitbox when attacking
    Rectangle GetAttackHitbox() const;

    // Starts a knockback effect when the player gets hit
    void StartKnockback(Vector2 velocity);

    // Checks if the player is currently attacking
    bool IsAttacking() const;

    // Player's health — starts at 5
    int health = 5;

    // Used to prevent the player from being hit repeatedly too quickly
    bool recentlyHit = false;
    float hitCooldown = 0.0f;
    const float hitCooldownDuration = 1.0f; // 1 second of invincibility after being hit

    // Knockback state — used when the player gets pushed back by an enemy
    bool isKnockback = false;
    Vector2 knockbackVelocity = { 0, 0 };
    float knockbackTimer = 0.0f;
    const float knockbackDuration = 0.2f; // lasts for 0.2 seconds

private:
    // The player's position in the world
    Vector2 position;

    // The invisible box used for collision detection
    Rectangle hitbox;

    // Movement speed in pixels per second
    float speed = 200.0f;

    // Size of the player character
    float width = 50;
    float height = 80;

    // Current movement direction and speed
    Vector2 velocity = { 0, 0 };

    // Gravity and jumping force
    float gravity = 800.0f;       // pulls the player down
    float jumpForce = -500.0f;    // pushes the player up

    // Whether the player is standing on the ground
    bool isOnGround = false;

    // Which way the player is facing
    bool facingRight = true;

    // Blinking animation (optional visual polish)
    bool isBlinking = false;
    float blinkTimer = 0.0f;
    float blinkInterval = 3.0f; // time between blinks
    float blinkDuration = 0.2f; // how long each blink lasts

    // Attacking state
    bool isAttacking = false;
    float attackTimer = 0.0f;
    const float attackDuration = 0.2f; // how long the attack lasts

    // Images used to show the frog in different states
    Texture2D frogIdle = { 0 };
    Texture2D frogIdleLeft = { 0 };
    Texture2D frogJumpRight = { 0 };
    Texture2D frogJumpLeft = { 0 };
    Texture2D attackRight = { 0 };
    Texture2D attackLeft = { 0 };
};