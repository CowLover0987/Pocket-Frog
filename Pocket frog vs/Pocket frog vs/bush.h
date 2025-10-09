#pragma once
#include "raylib.h"

class Bush {
public:
    Bush(Vector2 position);
    ~Bush();
    void Draw() const;
    Rectangle GetCollider() const;

private:
    Vector2 pos;
    Rectangle collider;
    Texture2D bushTexture = { 0 }; //Must be here
};