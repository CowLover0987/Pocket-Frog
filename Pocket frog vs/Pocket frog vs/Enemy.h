#pragma once // Makes sure this file is only included once during compilation

#include "raylib.h"     // Gives access to drawing tools and game types like Vector2 and Texture2D
#include <vector>       // Lets us use lists of bushes
#include "Bush.h"       // So enemies can check for bush collisions

// This is the blueprint for your woolly enemy character.
// It defines how enemies move, react to the player, and take damage.
class Enemy {
public:
    // This creates a new enemy at a specific position in the world
    Enemy(Vector2 startPos);

    // This updates the enemy's behavior every frame (movement, chasing, etc.)
    void Update(float dt, Vector2 playerPos, const std::vector<Bush>& bushes);

    // This draws the enemy on the screen
    void Draw() const;

    // This returns the enemy's hitbox — used for detecting attacks
    Rectangle GetCollider() const;

    // This returns the enemy's current position
    Vector2 GetPosition() const;

    // This handles movement and checks for bush collisions
    void TryMove(Vector2 delta, const std::vector<Bush>& bushes);

    // Optional: destructor (not used right now)
    //~Enemy();

    // This is called when the enemy gets hit by the player
    void TakeDamage();

    // This lets the game check if the enemy is still alive
    bool IsAlive() const;

private:
    // The enemy's position in the world
    Vector2 position;

    // The size of the enemy (used for drawing and collision)
    float width = 100;   // wider than a bush
    float height = 60;

    // How fast the enemy moves when wandering or chasing
    float speed = 40;         // slow wandering
    float chaseSpeed = 100;   // faster chasing
    float detectionRadius = 200; // how close the player must be to trigger chasing

    // The enemy's current movement direction and speed
    Vector2 velocity = { 0, 0 };

    // The images used to show the enemy facing left or right
    Texture2D enemyMoveRight = { 0 };
    Texture2D enemyMoveLeft = { 0 };
    bool facingRight = true; // Which way the enemy is facing

    // Health and life status
    int health = 1;       // Can be increased for tougher enemies
    bool isAlive = true;  // Becomes false when defeated
};