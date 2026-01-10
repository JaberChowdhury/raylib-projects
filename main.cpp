#include "raylib.h"

int main() {
    const int screenWidth  = 2048;
    const int screenHeight = 1152;

    // Define Colors
    const Color bg   = {164, 195, 178, 255};
    const Color text = {52, 77, 68, 255};

    InitWindow(screenWidth * 2, screenHeight * 2, "Raylib Shapes Example");
    SetTargetFPS(120);

    ChangeDirectory(GetApplicationDirectory());

    Font myfont = LoadFontEx("resources/zh-cn.ttf", 96, 0, 0);
    SetTextureFilter(myfont.texture, TEXTURE_FILTER_BILINEAR);

    // rendering loop
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(bg);

        DrawTextEx(myfont, "Welcome to my raylib project", {500, 1230}, 120, 1, text);
        EndDrawing();
    }

    // Cleanup
    UnloadFont(myfont);
    CloseWindow();
    return 0;
}
