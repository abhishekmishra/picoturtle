#include "raylib.h"

int main(void)
{
    // Initialization
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "Hello World - raylib");

    SetTargetFPS(60);

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Draw
        BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawText("Hello, World!", 350, 200, 20, DARKGRAY);
        EndDrawing();
    }

    // De-Initialization
    CloseWindow();

    return 0;
}