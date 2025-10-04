#pragma once
#include "raylib.h"

class Bush {
public:
    Bush(Vector2 position);
    void Draw() const;
    Rectangle GetCollider() const;

private:
    Vector2 pos;
    Rectangle collider;
};