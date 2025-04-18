#include <raylib.h>
#include "animation.h"

int main(void) {
    
    InitWindow(800, 600, "Raylib Example");
    char full_path[PATH_MAX];

    const char *filename = "asset.png";
    build_path_asset(full_path, filename);
    
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("Test Raylib Window", 10, 10, 20, DARKGRAY);
        EndDrawing();
    }
    CloseWindow();
    
    return 0;
}
