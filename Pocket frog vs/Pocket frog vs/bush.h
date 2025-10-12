#pragma once // This makes sure the file is only included once when compiling

#include "raylib.h" // This gives us access to drawing tools and game-related types like Vector2 and Texture2D

// This is the blueprint for a Bush object in the game.
// Bushes are little obstacles that the frog and enemies can't walk through.
class Bush {
public:
    // This is the function that creates a bush at a specific location
    Bush(Vector2 position);

    // Optional: a destructor that would clean up the bush (not used right now)
    //~Bush();

    // This function draws the bush on the screen
    void Draw() const;

    // This function lets other parts of the game check where the bush is
    Rectangle GetCollider() const;

private:
    // The bush's position in the world
    Vector2 pos;

    // The invisible box used for collision detection
    Rectangle collider;

    // The image used to show the bush on screen
    Texture2D bushTexture = { 0 }; // Must be initialized here
};