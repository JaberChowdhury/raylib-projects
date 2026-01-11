#include "CircleShape.h"
#include "raylib.h"
#include <string>
#include <vector>

// 1. Define Constants globally so the Class can see them
const int SCREEN_WIDTH  = 2048 * 2;
const int SCREEN_HEIGHT = 1152 * 2;

// 2. Define a struct for Assets/Theme (Colors and Fonts)
struct AppContext {
    Color bg;
    Color text;
    Font  myfont;
};

// 3. Helper function
void DrawTextCentered(const char* text, float y, float fontSize, float spacing, AppContext ctx) {
    Vector2 textSize = MeasureTextEx(ctx.myfont, text, fontSize, spacing);
    float   x        = (SCREEN_WIDTH - textSize.x) / 2.0f;
    DrawTextEx(ctx.myfont, text, {x, y}, fontSize, spacing, ctx.text);
}

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Raylib Moving Shapes");
    SetTargetFPS(120);
    ChangeDirectory(GetApplicationDirectory());

    // --- INITIALIZE CONTEXT ---
    // We must do this AFTER InitWindow because of the Font
    AppContext ctx;
    ctx.bg     = {164, 195, 178, 255};
    ctx.text   = {52, 77, 68, 255};
    ctx.myfont = LoadFontEx("resources/zh-cn.ttf", 96, 0, 0);

    SetTextureFilter(ctx.myfont.texture, TEXTURE_FILTER_BILINEAR);

    // --- CREATE OBJECT ---
    // Using stack allocation (no 'new') is safer and easier here
    CircleShape              myCircle(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 100.0f, 8.0f, 8.0f);
    std::vector<CircleShape> circles;

    // Loop 20 times to add circles
    for (int i = 0; i < 3000; i++) {
        float r  = GetRandomValue(2, 8);
        float x  = GetRandomValue(r, SCREEN_WIDTH - r);
        float y  = GetRandomValue(r, SCREEN_HEIGHT - r);
        float sx = GetRandomValue(-10, 10);
        float sy = GetRandomValue(-10, 10);

        // Create the circle and add it to the list
        circles.push_back(CircleShape(x, y, r, sx, sy));
    }
    while (!WindowShouldClose()) {

        // Draw
        BeginDrawing();
        ClearBackground(ctx.bg);

        // Pass the color from context to the draw function
        for (int i = 0; i < circles.size(); i++) {
            circles[i].update(SCREEN_WIDTH, SCREEN_HEIGHT);
            circles[i].draw(ctx.text);
        }
        // std::string coords =
        // "x: " + std::to_string((int) myCircle.position.x) + "  y: " + std::to_string((int) myCircle.position.y);

        DrawFPS(12, 12);
        DrawTextCentered("coords", 100, 60, 10, ctx);

        EndDrawing();
    }

    UnloadFont(ctx.myfont);
    CloseWindow();
    return 0;
}
