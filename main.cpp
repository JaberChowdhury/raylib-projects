#include "includes/GenerateMaterialColor.cpp"
#include "includes/OppositeColor.cpp"
#include "raylib.h"

Font genshinFont;
int  screenWidth  = 2560;
int  screenHeight = 1440;

void draw_text_center(float y, const char* text, Color color) {
    float   fontSize = 60;
    float   spacing  = 2;
    Vector2 textSize = MeasureTextEx(genshinFont, text, fontSize, spacing);
    float   posX     = ((float) screenWidth - textSize.x) / 2;

    DrawTextEx(genshinFont, text, {posX, y}, fontSize, spacing, color);
}

int main() {
    InitWindow(screenWidth, screenHeight, "Raylib - Material Color Generator");

    genshinFont = LoadFontEx("resources/zh-cn.ttf", 96, 0, 0);
    SetTextureFilter(genshinFont.texture, TEXTURE_FILTER_BILINEAR);

    int myHue = 210;
    SetTargetFPS(60);

    const int generated_colors_size = 10;
    Color     generated_colors[generated_colors_size];

    for (int i = 0; i < generated_colors_size; i++) {
        generated_colors[i] = GenerateMaterialColor(myHue, i * 100);
    }

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        float startY = ((float) screenHeight - (generated_colors_size * 70)) / 2;

        for (int i = 0; i < generated_colors_size; i++) {

            const char* text = TextFormat("Color Index: %d (Hue: %d)", i, myHue);

            draw_text_center((i * 70), text, generated_colors[i]);
            // draw_text_center((i * 70), text, OppositeColor(generated_colors[i]));
        }

        EndDrawing();
    }

    UnloadFont(genshinFont);
    CloseWindow();
    return 0;
}
