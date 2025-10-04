#include "Bush.h"

Bush::Bush(Vector2 position) {
    pos = position;
    collider = { pos.x, pos.y, 32, 32 }; // Adjust size as needed
}

void Bush::Draw() const {
    DrawRectangleRec(collider, GREEN);
}

Rectangle Bush::GetCollider() const {
    return collider;
}