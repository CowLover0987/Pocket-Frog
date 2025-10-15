#include <iostream>
#include <raylib.h>
#define MAX_INPUT_CHARS     3

using namespace std;

int main() {

    const int SCREEN_WIDTH = 1000;
    const int SCREEN_HEIGHT = 900;

    char red[MAX_INPUT_CHARS + 1] = "\0";      // NOTE: One extra space required for null terminator char '\0'
    char green[MAX_INPUT_CHARS + 1] = "\0";      // NOTE: One extra space required for null terminator char '\0'
    char blue[MAX_INPUT_CHARS + 1] = "\0";      // NOTE: One extra space required for null terminator char '\0'
    int letterCountRed = 0;
    int letterCountGreen = 0;
    int letterCountBlue = 0;

    Rectangle textBox_red = { SCREEN_WIDTH / 2.0f - 110, 480, 225, 50 };
    Rectangle textBox_green = { SCREEN_WIDTH / 2.0f - 110, 580, 225, 50 };
    Rectangle textBox_blue = { SCREEN_WIDTH / 2.0f - 110, 680, 225, 50 };
    bool mouseOnTextRed = false;
    bool mouseOnTextGreen = false;
    bool mouseOnTextBlue = false;

    int framesCounter = 0;

    int Red = {};
    int Blue = {};
    int Green = {};

    int* pointRed = &Red;
    int* pointGreen = &Green;
    int* pointBlue = &Blue;


    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "My first RAYLIB program!");
    SetTargetFPS(60);

    while (WindowShouldClose() == false) {

        if (CheckCollisionPointRec(GetMousePosition(), textBox_red)) mouseOnTextRed = true, mouseOnTextGreen = false, mouseOnTextBlue = false;
        else if (CheckCollisionPointRec(GetMousePosition(), textBox_green)) mouseOnTextGreen = true, mouseOnTextRed = false, mouseOnTextBlue = false;
        else if (CheckCollisionPointRec(GetMousePosition(), textBox_blue)) mouseOnTextBlue = true, mouseOnTextGreen = false, mouseOnTextRed = false;
        else mouseOnTextRed, mouseOnTextGreen, mouseOnTextBlue = false;

        if (mouseOnTextRed)
        {
            // Set the window's cursor to the I-Beam
            SetMouseCursor(MOUSE_CURSOR_IBEAM);

            // Get char pressed (unicode character) on the queue
            int key = GetCharPressed();

            // Check if more characters have been pressed on the same frame
            while (key > 0)
            {
                // NOTE: Only allow keys in range [32..125]
                if ((key >= 32) && (key <= 125) && (letterCountRed < MAX_INPUT_CHARS))
                {
                    red[letterCountRed] = (char)key;
                    red[letterCountRed + 1] = '\0'; // Add null terminator at the end of the string
                    letterCountRed++;
                }

                key = GetCharPressed();  // Check next character in the queue
            }

            if (IsKeyPressed(KEY_BACKSPACE))
            {
                letterCountRed--;
                if (letterCountRed < 0) letterCountRed = 0;
                red[letterCountRed] = '\0';
            }
        }
        else SetMouseCursor(MOUSE_CURSOR_DEFAULT);

        if (mouseOnTextGreen)
        {
            // Set the window's cursor to the I-Beam
            SetMouseCursor(MOUSE_CURSOR_IBEAM);

            // Get char pressed (unicode character) on the queue
            int key = GetCharPressed();

            // Check if more characters have been pressed on the same frame
            while (key > 0)
            {
                // NOTE: Only allow keys in range [32..125]
                if ((key >= 32) && (key <= 125) && (letterCountGreen < MAX_INPUT_CHARS))
                {
                    green[letterCountGreen] = (char)key;
                    green[letterCountGreen + 1] = '\0'; // Add null terminator at the end of the string
                    letterCountGreen++;
                }

                key = GetCharPressed();  // Check next character in the queue
            }

            if (IsKeyPressed(KEY_BACKSPACE))
            {
                letterCountGreen--;
                if (letterCountGreen < 0) letterCountGreen = 0;
                green[letterCountGreen] = '\0';
            }
        }
        else SetMouseCursor(MOUSE_CURSOR_DEFAULT);

        if (mouseOnTextBlue)
        {
            // Set the window's cursor to the I-Beam
            SetMouseCursor(MOUSE_CURSOR_IBEAM);

            // Get char pressed (unicode character) on the queue
            int key = GetCharPressed();

            // Check if more characters have been pressed on the same frame
            while (key > 0)
            {
                // NOTE: Only allow keys in range [32..125]
                if ((key >= 32) && (key <= 125) && (letterCountBlue < MAX_INPUT_CHARS))
                {
                    blue[letterCountBlue] = (char)key;
                    blue[letterCountBlue + 1] = '\0'; // Add null terminator at the end of the string
                    letterCountBlue++;
                }

                key = GetCharPressed();  // Check next character in the queue
            }

            if (IsKeyPressed(KEY_BACKSPACE))
            {
                letterCountBlue--;
                if (letterCountBlue < 0) letterCountBlue = 0;
                blue[letterCountBlue] = '\0';
            }
        }
        else SetMouseCursor(MOUSE_CURSOR_DEFAULT);

        if (mouseOnTextRed || mouseOnTextGreen || mouseOnTextBlue) framesCounter++;
        else framesCounter = 0;


        BeginDrawing();
        ClearBackground(BLACK);

        //Red Colour Input
        DrawRectangleRec(textBox_red, LIGHTGRAY);
        if (mouseOnTextRed) DrawRectangleLines((int)textBox_red.x, (int)textBox_red.y, (int)textBox_red.width, (int)textBox_red.height, RED);
        else DrawRectangleLines((int)textBox_red.x, (int)textBox_red.y, (int)textBox_red.width, (int)textBox_red.height, DARKGRAY);

        DrawText(red, (int)textBox_red.x + 5, (int)textBox_red.y + 8, 40, MAROON);

        //Checking if input exceeds the 3 characters
        if (mouseOnTextRed)
        {
            if (mouseOnTextRed < MAX_INPUT_CHARS)
            {
                // Draw blinking underscore char
                if (((framesCounter / 20) % 2) == 0) DrawText("_", (int)textBox_red.x + 8 + MeasureText(red, 40), (int)textBox_red.y + 12, 40, MAROON);
            }
            else DrawText("Press BACKSPACE to delete chars...", 230, 300, 20, GRAY);
        }


        //Green Colour Input
        DrawRectangleRec(textBox_green, LIGHTGRAY);
        if (mouseOnTextGreen) DrawRectangleLines((int)textBox_green.x, (int)textBox_green.y, (int)textBox_green.width, (int)textBox_green.height, RED);
        else DrawRectangleLines((int)textBox_green.x, (int)textBox_green.y, (int)textBox_green.width, (int)textBox_green.height, DARKGRAY);

        DrawText(green, (int)textBox_green.x + 5, (int)textBox_green.y + 8, 40, MAROON);

        //Checking if input exceeds the 3 characters
        if (mouseOnTextGreen)
        {
            if (mouseOnTextGreen < MAX_INPUT_CHARS)
            {
                // Draw blinking underscore char
                if (((framesCounter / 20) % 2) == 0) DrawText("_", (int)textBox_green.x + 8 + MeasureText(green, 40), (int)textBox_green.y + 12, 40, MAROON);
            }
            else DrawText("Press BACKSPACE to delete chars...", 230, 300, 20, GRAY);
        }


        //Green Colour Input
        DrawRectangleRec(textBox_blue, LIGHTGRAY);
        if (mouseOnTextBlue) DrawRectangleLines((int)textBox_blue.x, (int)textBox_blue.y, (int)textBox_blue.width, (int)textBox_blue.height, RED);
        else DrawRectangleLines((int)textBox_blue.x, (int)textBox_blue.y, (int)textBox_blue.width, (int)textBox_blue.height, DARKGRAY);

        DrawText(blue, (int)textBox_blue.x + 5, (int)textBox_blue.y + 8, 40, MAROON);

        //Checking if input exceeds the 3 characters
        if (mouseOnTextBlue)
        {
            if (mouseOnTextBlue < MAX_INPUT_CHARS)
            {
                // Draw blinking underscore char
                if (((framesCounter / 20) % 2) == 0) DrawText("_", (int)textBox_blue.x + 8 + MeasureText(blue, 40), (int)textBox_blue.y + 12, 40, MAROON);
            }
            else DrawText("Press BACKSPACE to delete chars...", 230, 300, 20, GRAY);
        }

        *pointRed = atoi(red);
        *pointGreen = atoi(green);
        *pointBlue = atoi(blue);


        Color InputColour{ Red, Green, Blue, 225 };

        DrawRectangle(300, 100, 200, 200, InputColour);

        EndDrawing();
    }

    CloseWindow();
}

// Check if any key is pressed 
// NOTE: We limit keys check to keys between 32 (KEY_SPACE) and 126
bool IsAnyKeyPressed()
{
    bool keyPressed = false;
    int key = GetKeyPressed();

    if ((key >= 32) && (key <= 126)) keyPressed = true;

    return keyPressed;
}