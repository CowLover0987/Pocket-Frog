// This file controls how enemies behave in the game.
// They can move, detect the player, take damage, and be drawn on screen.

#include "Enemy.h"     // Includes the enemy class definition
#include <cmath>       // Lets us use square roots and other math functions
#include "Global.h"    // Shared game data like level boundaries
#include <iostream>    // Lets us print messages for debugging

// A helper function that keeps a number within a safe range
inline float Clamp(float value, float min, float max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

// This lets other parts of the game ask where the enemy is
Vector2 Enemy::GetPosition() const {
    return position;
}

// This runs when a new enemy is created
Enemy::Enemy(Vector2 startPos) {
    position = startPos; // Set the enemy's starting position

    // Load the images for the enemy facing left and right
    enemyMoveRight = LoadTexture("Resource Files/enemy_right.png");
    enemyMoveLeft = LoadTexture("Resource Files/enemy_left.png");

    // If the images fail to load, print an error message
    if (enemyMoveRight.id == 0) std::cerr << "Failed to load evil_wool_right.png\n";
    if (enemyMoveLeft.id == 0) std::cerr << "Failed to load evil_wool_left.png\n";
}

// This updates the enemy's movement and behavior every frame
void Enemy::Update(float dt, Vector2 playerPos, const std::vector<Bush>& bushes) {
    // Figure out how far the player is from the enemy
    Vector2 toPlayer = { playerPos.x - position.x, playerPos.y - position.y };
    float distance = std::sqrt(toPlayer.x * toPlayer.x + toPlayer.y * toPlayer.y);

    // Face the direction the enemy is moving
    if (velocity.x > 0) facingRight = true;
    else if (velocity.x < 0) facingRight = false;

    // If the player is close enough, chase them
    if (distance < detectionRadius) {
        // Only move left or right (not up/down)
        float length = std::sqrt(toPlayer.x * toPlayer.x);
        if (length != 0) {
            velocity.x = (toPlayer.x / length) * chaseSpeed;
        }
        velocity.y = 0;
    }
    else {
        // If the player is far away, wander back and forth slowly
        velocity.x = std::sin(GetTime()) * speed;
        velocity.y = 0;
    }

    // Move the enemy based on its velocity
    Vector2 move = { velocity.x * dt, 0 };
    TryMove(move, bushes);
}

// This draws the enemy on the screen
void Enemy::Draw() const {
    // Choose the correct image based on which way the enemy is facing
    const Texture2D* enemyTexture = facingRight ? &enemyMoveRight : &enemyMoveLeft;

    // Figure out where to draw the image so it lines up with the enemy's position
    Vector2 drawPos = {
        position.x - enemyTexture->width / 2,
        position.y - enemyTexture->height
    };

    // Draw the enemy image
    DrawTexture(*enemyTexture, (int)drawPos.x, (int)drawPos.y, WHITE);

    // Draw the enemy's collision box so you can see where it can be hit
    Rectangle collider = GetCollider();
    DrawRectangleRec(collider, Fade(RED, 0.3f));         // semi-transparent red box
    DrawRectangleLinesEx(collider, 1, DARKGRAY);         // dark outline
}

// This returns the enemy's hitbox — the area where it can be attacked
Rectangle Enemy::GetCollider() const {
    return { position.x - 50, position.y - 50, width, height };
}

// This handles movement and collision with bushes
void Enemy::TryMove(Vector2 delta, const std::vector<Bush>& bushes) {
    // Predict where the enemy wants to move
    Rectangle proposed = { position.x + delta.x, position.y + delta.y, width, height };

    // Keep the enemy inside the level boundaries
    //proposed.x = Clamp(proposed.x, levelBounds.x, levelBounds.x + levelBounds.width - width);
    proposed.y = Clamp(proposed.y, levelBounds.y, levelBounds.y + levelBounds.height - height);

    // Check if the enemy would bump into a bush
    for (const Bush& bush : bushes) {
        if (CheckCollisionRecs(proposed, bush.GetCollider())) {
            return; // Stop moving if blocked
        }
    }

    // Move the enemy to the new position
    position.x = proposed.x;
    position.y = proposed.y;
}

// This is called when the enemy gets hit by the player
void Enemy::TakeDamage() {
    health--; // Reduce health by 1
    if (health <= 0) {
        isAlive = false; // Mark the enemy as defeated
        std::cout << "Enemy defeated!\n"; // Print a message for debugging
    }
    else {
        std::cout << "Enemy took damage. Health now: " << health << "\n";
    }
}

// This lets the game check if the enemy is still alive
bool Enemy::IsAlive() const {
    return isAlive;
}