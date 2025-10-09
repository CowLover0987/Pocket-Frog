#include "raylib.h"
#include <memory>
#include "Player.h"
#include "bush.h"
#include "Global.h"
#include "Enemy.h"

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

void InitGame() {
    InitWindow(screenWidth, screenHeight, "Pocket Frog");
    SetTargetFPS(60);

    camera.offset = Vector2{ screenWidth / 2.0f, screenHeight / 2.0f };
    camera.zoom = 1.0f;

    camera.target.x = Clamp(camera.target.x, levelBounds.x + halfScreenW, levelBounds.x + levelBounds.width - halfScreenW);
    camera.target.y = Clamp(camera.target.y, levelBounds.y + halfScreenH, levelBounds.y + levelBounds.height - halfScreenH);


    player = std::make_unique<Player>(Vector2{ 100, 520 });

	//Bushes spawned locations, will need to be adjusted later
    bushes.push_back(Bush({ 400, 500 }));
    bushes.push_back(Bush({ 600, 500 }));
    //Ememys spawned locations, will need to be adjusted later
    enemies.push_back(Enemy({ 800, 490 }));
}

void UpdateGame(float dt) {
    player->Update(dt, bushes);
    for (Enemy& enemy : enemies) {
        enemy.Update(dt, player->GetPosition(), bushes);
        // Check collision with player
        if (CheckCollisionRecs(enemy.GetCollider(), player->GetHitbox())) {
            DrawText("Game Over!", 500, 300, 40, RED);
            WaitTime(1.0f); // pause briefly
            CloseWindow();
        }
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

    CloseWindow();
    return 0;
}