#include "raylib.h"

int main() {
    Color colors[] = {LIGHTGRAY, GRAY,  DARKGRAY,  YELLOW,  GOLD,  ORANGE,   PINK,    RED,     MAROON,
                      GREEN,     LIME,  DARKGREEN, SKYBLUE, BLUE,  DARKBLUE, PURPLE,  VIOLET,  DARKPURPLE,
                      BEIGE,     BROWN, DARKBROWN, WHITE,   BLACK, BLANK,    MAGENTA, RAYWHITE};

    int colorCount = sizeof(colors) / sizeof(colors[0]);

    const int screenWidth  = 2048;
    const int screenHeight = 1152;

    InitWindow(screenWidth, screenHeight, "My first raylib project");
    SetTargetFPS(120);

    ChangeDirectory(GetApplicationDirectory());

    Font myfont = LoadFontEx("resources/zh-cn.ttf", 96, 0, 0);
    SetTextureFilter(myfont.texture, TEXTURE_FILTER_BILINEAR);

    int distance = 40;

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        for (int x = 0; x < screenWidth; x += distance) {
            for (int y = 0; y < screenHeight; y += distance) {

                Vector2 pos = {(float) x, (float) y};

                int colorIndex = (x / distance) % colorCount;

                DrawTextEx(myfont, "0", pos, 40, 2, colors[colorIndex]);
            }
        }

        EndDrawing();
    }

    // 4. Cleanup
    UnloadFont(myfont);
    CloseWindow();

    return 0;
}
