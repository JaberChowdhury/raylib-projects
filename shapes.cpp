#include "raylib.h"
#include <string>

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
    int x = 0;
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(bg);

        DrawTextEx(myfont, "Welcome to my raylib project", {500, 1230}, 120, 1, text);
        // --- 1. SQUARE & RECTANGLE ---
        // DrawRectangle(x, y, width, height, color)
        DrawRectangle(100, 200, 200, 200, text); // Square (equal width/height)
        DrawTextEx(myfont, "Square", {100, 150}, 40, 2, text);

        DrawRectangle(400, 200, 300, 150, text); // Rectangle
        DrawTextEx(myfont, "Rect", {400, 150}, 40, 2, text);

        // --- 2. CIRCLE ---
        // DrawCircle(centerX, centerY, radius, color)
        DrawCircle(900, 300, 150, text);
        DrawCircle(900, 300, 100, bg);
        DrawTextEx(myfont, "Circle", {840, 100}, 40, 2, text);

        // --- 3. TRIANGLE ---
        // DrawTriangle(v1, v2, v3, color)
        // Points must be defined in counter-clockwise order for it to draw
        // correctly
        Vector2 v1 = {1100, 400}; // Bottom Left
        Vector2 v2 = {1300, 400}; // Bottom Right
        Vector2 v3 = {1200, 200}; // Top Center
        DrawTriangle(v1, v2, v3, text);
        DrawTextEx(myfont, "Triangle", {1120, 150}, 40, 2, text);

        // --- 4. POLYGON (Hexagon) ---
        // DrawPoly(center, sides, radius, rotation, color)
        //
        if (x > 1000)
            x = 0;
        DrawPoly({1500, 300}, 5, 100, x++, text);
        DrawTextEx(myfont, ("Poly" + std::to_string(x)).c_str(), {1450, 150}, 40, 2, text);

        // --- 5. MESH (2D Grid Representation) ---
        // Actual Meshes are 3D. In 2D, we simulate a mesh using lines or small
        // rects.
        int startX   = 100;
        int startY   = 600;
        int cellSize = 40;
        int rows     = 5;
        int cols     = 10;

        DrawTextEx(myfont, "Mesh / Grid", {100, 550}, 40, 2, text);

        // Draw the grid lines
        for (int i = 0; i <= cols; i++) {
            DrawLine(startX + (i * cellSize), startY, startX + (i * cellSize), startY + (rows * cellSize), text);
        }
        for (int i = 0; i <= rows; i++) {
            DrawLine(startX, startY + (i * cellSize), startX + (cols * cellSize), startY + (i * cellSize), text);
        }

        // --- 6. ROUNDED RECTANGLE (Bonus) ---
        DrawRectangleRounded({700, 600, 300, 200}, 0.3f, 62, text);
        DrawTextEx(myfont, "Rounded", {700, 550}, 40, 2, text);

        EndDrawing();
    }

    // Cleanup
    UnloadFont(myfont);
    CloseWindow();

    return 0;
}
