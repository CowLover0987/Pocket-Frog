// These are the tools and libraries the game uses.
// raylib helps with drawing and input, memory helps manage objects,
// and the other files define your frog, bushes, enemies, and shared data.
#include "raylib.h"
#include <memory>
#include "Player.h"
#include "bush.h"
#include "Global.h"
#include "Enemy.h"
#include <string>
#include <fstream>
#include <iostream>

static float nextSpawnX = 1000.0f;

// Set the size of the game window
const int screenWidth = 1280;
const int screenHeight = 720;

//How for the player has moved to the right
float playerProgressX = 0.0f;

// Create the player, bushes, and enemies
std::unique_ptr<Player> player;
std::vector<Bush> bushes;
std::vector<Enemy> enemies;

// Define the size of the world the player can move around in
Rectangle levelBounds = { 0, 0, playerProgressX + screenWidth, 720 }; // left, top, width, height

// Set up the camera that follows the player
Camera2D camera = { 0 };
float halfScreenW = screenWidth / 2.0f;
float halfScreenH = screenHeight / 2.0f;

// A helper function to keep values within a safe range
template <typename T>
inline T Clamp(T value, T min, T max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

// These helpers do simple math with positions
inline Vector2 Vector2Subtract(Vector2 v1, Vector2 v2) {
    return { v1.x - v2.x, v1.y - v2.y };
}

inline Vector2 Vector2Normalize(Vector2 v) {
    float length = sqrtf(v.x * v.x + v.y * v.y);
    if (length > 0.0f) {
        return { v.x / length, v.y / length };
    }
    return { 0.0f, 0.0f };
}

inline Vector2 Vector2Scale(Vector2 v, float scale) {
    return { v.x * scale, v.y * scale };
}

//Spawn bushes and enemies as the player moves right
void SpawnEnvironment(float playerX) {
    while (nextSpawnX < playerX + 1000.0f) {
        if (GetRandomValue(0, 3) < 3) {
            bushes.push_back(Bush({ nextSpawnX, 490 }));
        }

        if (GetRandomValue(0, 3) < 3) {
            enemies.push_back(Enemy({ nextSpawnX + GetRandomValue(50, 150), 490 }));
        }

        nextSpawnX += GetRandomValue(150, 300); // more frequent spawns
    }
}

// Save how many enemies the player has defeated to a file
void SaveProgress() {
    std::cout << "Saving to: save.txt\n";
    std::ofstream saveFile("save.txt");
    if (saveFile.is_open()) {
        saveFile << highScore;
        saveFile.close();
    }
}

// Set up the game when it starts
void InitGame() {
    InitWindow(screenWidth, screenHeight, "Pocket Frog");
    SetTargetFPS(60); // Run at 60 frames per second

    // Load saved progress from a file
    std::ifstream loadFile("save.txt");
    if (loadFile.is_open()) {
        loadFile >> highScore;
        loadFile.close();
    }

    // Set up the camera to center on the screen
    camera.offset = Vector2{ screenWidth / 2.0f, screenHeight / 2.0f };
    camera.zoom = 1.0f;

    // Make sure the camera doesn't go outside the world
    //camera.target.x = Clamp(camera.target.x, levelBounds.x + halfScreenW, levelBounds.x + levelBounds.width - halfScreenW);
    camera.target.y = Clamp(camera.target.y, levelBounds.y + halfScreenH, levelBounds.y + levelBounds.height - halfScreenH);

    // Create the player frog at a starting position
    player = std::make_unique<Player>(Vector2{ 100, 520 });

    // Add bushes to the world
    bushes.push_back(Bush({ 400, 490 }));
    bushes.push_back(Bush({ 600, 490 }));

    // Add one enemy to start with
    //enemies.push_back(Enemy({ 800, 490 }));
}

// Update the game logic every frame
void UpdateGame(float dt) {
    // Update the player (movement, jumping, attacking)
    player->Update(dt, bushes);
    playerProgressX = player->GetPosition().x;
    SpawnEnvironment(playerProgressX);

    // If the player is swinging their sword, check for enemy hits
    if (player->IsAttacking()) {
        Rectangle swordHitbox = player->GetAttackHitbox();
        for (Enemy& enemy : enemies) {
            if (enemy.IsAlive() && CheckCollisionRecs(swordHitbox, enemy.GetCollider())) {
                enemy.TakeDamage();
                if (!enemy.IsAlive()) {
                    enemiesDefeated++; // Count the defeat
                }
            }
        }
    }

    // Remove enemies that are no longer alive
    enemies.erase(
        std::remove_if(enemies.begin(), enemies.end(),
            [](const Enemy& e) { return !e.IsAlive(); }),
        enemies.end()
    );

    // Update each enemy and check if they hit the player
    for (Enemy& enemy : enemies) {
        enemy.Update(dt, player->GetPosition(), bushes);

        // If the enemy touches the player and the player isn't invincible
        if (enemy.IsAlive() && CheckCollisionRecs(enemy.GetCollider(), player->GetHitbox())) {
            if (!player->recentlyHit) {
                player->health--; // Reduce player health
                player->recentlyHit = true;
                player->hitCooldown = player->hitCooldownDuration;

                // Push the player away from the enemy
                float knockbackSpeed = 300.0f;
                Vector2 dir = Vector2Normalize(Vector2Subtract(player->GetPosition(), enemy.GetPosition()));
                player->StartKnockback(Vector2Scale(dir, knockbackSpeed));
            }
        }
        if (!enemy.IsAlive()) {
            if (enemiesDefeated > highScore) {
                highScore = enemiesDefeated;
                SaveProgress(); // Save immediately when high score is beaten
            }
        }

    }

    // If the player's health reaches zero, end the game
    if (player->health <= 0) {
        DrawText("Game Over!", 500, 300, 40, RED);
        SaveProgress();
        WaitTime(1.0f);
        CloseWindow();
    }

    //float cleanupX = playerProgressX - 800;

    //bushes.erase(std::remove_if(bushes.begin(), bushes.end(),
    //    [cleanupX](const Bush& b) { return b.GetPosition().x < cleanupX; }), bushes.end());

    //enemies.erase(std::remove_if(enemies.begin(), enemies.end(),
    //    [cleanupX](const Enemy& e) {return e.GetPosition().x < cleanupX && !e.IsAlive();}), enemies.end());

}

// Draw everything on the screen
void DrawGame() {
    BeginDrawing();
    ClearBackground(DARKGRAY); // Set background color

    BeginMode2D(camera); // Start drawing with camera movement

    // Draw the ground and a red line to show where it is
    DrawRectangle(0, groundY, playerProgressX + screenWidth, screenHeight - groundY, DARKBROWN);
    //DrawLine(0, groundY, 3000, groundY, RED);

    // Draw the player frog
    player->Draw();

    // Draw all bushes
    for (const Bush& bush : bushes) {
        bush.Draw();
    }

    // Draw all enemies
    for (const Enemy& enemy : enemies) {
        enemy.Draw();
    }

    // Draw the edges of the world
    //DrawRectangleLinesEx(levelBounds, 2, RED);

    EndMode2D(); // Stop camera drawing

    // Draw the heads-up display (HUD)
    DrawText("Welcome to your pocket world", 20, 20, 30, WHITE);

    // Show how many enemies have been defeated
    std::string counterText = "Enemies Defeated: " + std::to_string(enemiesDefeated);
    int textWidth = MeasureText(counterText.c_str(), 20);
    DrawText(counterText.c_str(), GetScreenWidth() - textWidth - 20, 20, 20, DARKGREEN);
    DrawText(TextFormat("High Score: %d", highScore), 20, 110, 20, GOLD);


    // Show the player's current health
    DrawText(TextFormat("Health: %d", player->health), 20, 50, 20, RED);

    EndDrawing(); // Finish drawing everything
}

// The main function that runs the game
int main() {
    InitGame(); // Set everything up

    // Keep running until the player closes the window
    while (!WindowShouldClose()) {
        float dt = GetFrameTime(); // Time since last frame
        camera.target = player->GetPosition(); // Follow the player
        UpdateGame(dt); // Update game logic
        DrawGame();     // Draw everything
    }

    CloseWindow(); // Close the game window
    if (IsKeyPressed(KEY_ESCAPE)) {
        SaveProgress();
        CloseWindow(); // or set gameState to exit
    }
    return 0;
}