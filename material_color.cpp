#include "includes/GenerateMaterialColor.cpp"
#include "raylib.h"
#include <cmath>

int main() {
    const int screenWidth  = 2560;
    const int screenHeight = 1440;
    InitWindow(screenWidth, screenHeight, "Raylib - Material Color Generator");

    Font genshinFont = LoadFontEx("resources/zh-cn.ttf", 96, 0, 0);
    SetTextureFilter(genshinFont.texture, TEXTURE_FILTER_BILINEAR);

    int myHue       = 110;
    int shades[]    = {50, 100, 200, 300, 400, 500, 600, 700, 800, 900};
    int shadesCount = sizeof(shades) / sizeof(shades[0]);

    SetTargetFPS(60);

    const int generated_colors_size = 10;
    Color     generated_colors[generated_colors_size];

    for (int i = 0; i < generated_colors_size; i++) {
        generated_colors[i] = GenerateMaterialColor(myHue, shades[i]);
    }

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        int startY       = 50;
        int heightPerBar = 40;
        int widthBar     = 600;
        int startX       = (screenWidth - widthBar) / 2;

        DrawTextEx(genshinFont, TextFormat("Material Palette for Hue: %d", myHue), {(float) startX, 20}, 60, 2,
                   DARKGRAY);

        for (int i = 0; i < generated_colors_size; i++) {
            int yPos = startY + (i * (heightPerBar + 5));
            DrawRectangle(startX, yPos + 30, 1400, 90, generated_colors[i]);
            Color textColor = (shades[i] > 400) ? WHITE : BLACK;
        }
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
