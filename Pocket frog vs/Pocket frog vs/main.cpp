#include "raylib.h"
#include <memory>
#include "Player.h"
#include "bush.h"
#include "Global.h"
#include "Enemy.h"
#include <string>
#include <fstream>

// Constants
const int screenWidth = 1280;
const int screenHeight = 720;
std::unique_ptr<Player> player;
std::vector<Bush> bushes;
std::vector<Enemy> enemies;
Rectangle levelBounds = { 0, 0, 3000, 720 }; // x, y, width, height

// Camera
Camera2D camera = { 0 };
float halfScreenW = screenWidth / 2.0f;
float halfScreenH = screenHeight / 2.0f;

template <typename T>
inline T Clamp(T value, T min, T max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

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

void SaveProgress() {
    std::ofstream saveFile("save.txt");
    if (saveFile.is_open()) {
        saveFile << enemiesDefeated;
        saveFile.close();
    }
}

void InitGame() {
    InitWindow(screenWidth, screenHeight, "Pocket Frog");
    SetTargetFPS(60);

    std::ifstream loadFile("save.txt");
    if (loadFile.is_open()) {
        loadFile >> enemiesDefeated;
        loadFile.close();
    }

    camera.offset = Vector2{ screenWidth / 2.0f, screenHeight / 2.0f };
    camera.zoom = 1.0f;

    camera.target.x = Clamp(camera.target.x, levelBounds.x + halfScreenW, levelBounds.x + levelBounds.width - halfScreenW);
    camera.target.y = Clamp(camera.target.y, levelBounds.y + halfScreenH, levelBounds.y + levelBounds.height - halfScreenH);


    player = std::make_unique<Player>(Vector2{ 100, 520 });

	//Bushes spawned locations, will need to be adjusted later
    bushes.push_back(Bush({ 400, 490 }));
    bushes.push_back(Bush({ 600, 490 }));
    //Ememys spawned locations, will need to be adjusted later
    enemies.push_back(Enemy({ 800, 490 }));
}

void UpdateGame(float dt) {
    player->Update(dt, bushes);
    if (player->IsAttacking()) {
        Rectangle swordHitbox = player->GetAttackHitbox();
        for (Enemy& enemy : enemies) {
            if (enemy.IsAlive() && CheckCollisionRecs(swordHitbox, enemy.GetCollider())) {
                enemy.TakeDamage();
                if (!enemy.IsAlive()) {
                    enemiesDefeated++;
                }
            }
        }
    }

    // Remove dead enemies
    enemies.erase(
        std::remove_if(enemies.begin(), enemies.end(),
            [](const Enemy& e) { return !e.IsAlive(); }),
        enemies.end()
    );

    for (Enemy& enemy : enemies) {
        enemy.Update(dt, player->GetPosition(), bushes);

        if (enemy.IsAlive() && CheckCollisionRecs(enemy.GetCollider(), player->GetHitbox())) {
            if (!player->recentlyHit) {
                player->health--;
                player->recentlyHit = true;
                player->hitCooldown = player->hitCooldownDuration;

                // Calculate knockback direction
                float knockbackSpeed = 300.0f;
                Vector2 dir = Vector2Normalize(Vector2Subtract(player->GetPosition(), enemy.GetPosition()));
                player->StartKnockback(Vector2Scale(dir, knockbackSpeed));
            }
        }
    }

    if (player->health <= 0) {
        DrawText("Game Over!", 500, 300, 40, RED);
        WaitTime(1.0f);
        CloseWindow();
    }
}



void DrawGame() {
    BeginDrawing();
    ClearBackground(DARKGRAY);

    BeginMode2D(camera);

    

    // Draw ground
    DrawRectangle(0, groundY, 3000, screenHeight - groundY, DARKBROWN);
    DrawLine(0, groundY, 3000, groundY, RED); // horizontal ground line

    player->Draw();
    

    for (const Bush& bush : bushes) {
        bush.Draw();
    }

    for (const Enemy& enemy : enemies) {
        enemy.Draw();
    }

    DrawRectangleLinesEx(levelBounds, 2, RED);

    EndMode2D();

    // HUD (optional for now)
    DrawText("Welcome to your pocket world", 20, 20, 30, WHITE);
    std::string counterText = "Enemies Defeated: " + std::to_string(enemiesDefeated);
    int textWidth = MeasureText(counterText.c_str(), 20);
    DrawText(counterText.c_str(), GetScreenWidth() - textWidth - 20, 20, 20, DARKGREEN);
    DrawText(TextFormat("Health: %d", player->health), 20, 50, 20, RED);
    
    EndDrawing();
}

int main() {
    InitGame();

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();
        camera.target = player->GetPosition();
        UpdateGame(dt);
        DrawGame();
    }
    if (IsKeyPressed(KEY_S)) {
        SaveProgress();
    }
    CloseWindow();
    return 0;
}