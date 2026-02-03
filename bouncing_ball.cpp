#include "includes/GenerateMaterialColor.cpp"
#include "raylib.h"
#include <cstddef>
#include <ctime>
#include <string>

const int screenWidth  = 2048;
const int screenHeight = 1152;

// Define Colors
const Color bg   = {164, 195, 178, 255};
const Color text = {52, 77, 68, 255};

void DrawTextCentered(Font font, const char* text, float y, float fontSize, float spacing, Color color) {
    Vector2 textSize = MeasureTextEx(font, text, fontSize, spacing);
    float   x        = (screenWidth - textSize.x) / 2.0f;
    DrawTextEx(font, text, {x, y}, fontSize, spacing, color);
}

class CircleShape {
  public:
    Vector2 position;
    Vector2 velocity; // We need velocity to control movement direction
    float   radius;
    Color   color = GenerateMaterialColor(100, 200);

    // Constructor to initialize the circle easily
    CircleShape(float x, float y, float r, float speedX, float speedY) {
        position = {x, y};
        radius   = r;
        velocity = {speedX, speedY};
    }

    void draw() { DrawCircleV(position, radius, this->color); }

    void update() {
        // 1. Move the circle
        position.x += velocity.x;
        position.y += velocity.y;

        // 2. Bounce off the Right or Left wall

        if ((position.x + radius >= screenWidth) || (position.x - radius <= 0)) {
            velocity.x *= -1; // Reverse horizontal direction
            this->color = GenerateMaterialColor(GetRandomValue(12, 100), GetRandomValue(12, 500));
        }

        // 3. Bounce off the Bottom or Top wall
        SetRandomSeed(std::time(NULL));
        if ((position.y + radius >= screenHeight) || (position.y - radius <= 0)) {
            velocity.y *= -1; // Reverse vertical direction
            this->color = GenerateMaterialColor(GetRandomValue(12, 100), GetRandomValue(12, 500));
        }
    }
};

int main() {
    InitWindow(screenWidth, screenHeight, "Raylib Moving Shapes");
    SetTargetFPS(120);
    ChangeDirectory(GetApplicationDirectory());

    Font myfont = LoadFontEx("resources/zh-cn.ttf", 96, 0, 0);
    SetTextureFilter(myfont.texture, TEXTURE_FILTER_BILINEAR);

    // --- CREATE THE CIRCLE OBJECT ---
    // Start at center, radius 100, moving at speed (5, 5)
    CircleShape* myCircle = new CircleShape(screenWidth / 2.0f, screenHeight / 2.0f, 100.0f, 8.0f, 8.0f);

    while (!WindowShouldClose()) {
        // --- UPDATE LOGIC ---
        myCircle->update();

        // --- DRAWING ---
        BeginDrawing();
        ClearBackground(bg);

        myCircle->draw();

        // Display the coordinates of the moving circle
        std::string coords =
            "x: " + std::to_string((int) myCircle->position.x) + "  y: " + std::to_string((int) myCircle->position.y);

        DrawTextCentered(myfont, coords.c_str(), 100, 60, 10, text);

        EndDrawing();
    }

    UnloadFont(myfont);
    CloseWindow();
    return 0;
}
