#include "Bush.h"
#include <iostream>

Bush::Bush(Vector2 position) {
    pos = position;
    collider = { pos.x, pos.y, 32, 32 }; // Adjust size as needed

    bushTexture = LoadTexture("Resource Files/bush.png");

    if (bushTexture.id == 0) {
        std::cerr << "Failed to load bush.png\n";
    }
}

void Bush::Draw() const {
    //DrawTexture(bushTexture, (int)collider.x, (int)collider.y, WHITE);
	DrawRectangle((int)collider.x, (int)collider.y, (int)collider.width, (int)collider.height, GREEN);
}

Rectangle Bush::GetCollider() const {
    return collider;
}


Bush::~Bush() {
    UnloadTexture(bushTexture);
}