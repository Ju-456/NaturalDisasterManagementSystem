#include "animation.h"

int main_window(){

    InitWindow(800, 600, "Raylib Example");
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("Test Raylib Window", 10, 10, 20, DARKGRAY);
        EndDrawing();
    }
    CloseWindow();
}