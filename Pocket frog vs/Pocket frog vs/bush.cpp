// This file defines how bushes behave in the game.
// Bushes are little obstacles that the frog and enemies can't walk through.

#include "Bush.h"       // Includes the bush class definition
#include <iostream>     // Lets us print error messages if something goes wrong

// This is the constructor — it runs when a new bush is created.
Bush::Bush(Vector2 position) {
    pos = position; // Save the bush's position in the world

    // Create a box around the bush that other things can't walk through
    collider = { pos.x, pos.y, 32, 32 }; // The bush is 32x32 pixels in size

    // Load the image that shows what the bush looks like
    bushTexture = LoadTexture("Resource Files/bush.png");

    // If the image didn't load correctly, print an error message
    if (bushTexture.id == 0) {
        std::cerr << "Failed to load bush.png\n";
    }
}

// This function draws the bush on the screen
void Bush::Draw() const {
    DrawTexture(bushTexture, (int)collider.x, (int)collider.y, WHITE); // Draw the bush image

    // Optional: draw a green box to show the bush's collision area (for debugging)
    // DrawRectangle((int)collider.x, (int)collider.y, (int)collider.width, (int)collider.height, GREEN);
}

// This function lets other parts of the game check where the bush is
// and whether something is bumping into it
Rectangle Bush::GetCollider() const {
    return collider;
}