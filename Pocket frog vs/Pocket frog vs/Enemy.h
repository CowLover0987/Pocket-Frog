#pragma once
#include "raylib.h"
#include <vector>
#include "Bush.h"

class Enemy {
public:
    Enemy(Vector2 startPos);
    void Update(float dt, Vector2 playerPos, const std::vector<Bush>& bushes);
    void Draw() const;
    Rectangle GetCollider() const;

    void TryMove(Vector2 delta, const std::vector<Bush>& bushes);

    //~Enemy();

    void TakeDamage();
    bool IsAlive() const;

private:
    Vector2 position;
    float width = 96;   // bigger than bush
    float height = 60;
    float speed = 40;   // wander speed
    float chaseSpeed = 100;
    float detectionRadius = 200;
    Vector2 velocity = { 0, 0 };

    Texture2D enemyMoveRight = { 0 };
    Texture2D enemyMoveLeft = { 0 };
    bool facingRight = true;

    int health = 1; // or more, if you want multi-hit enemies
    bool isAlive = true;
};