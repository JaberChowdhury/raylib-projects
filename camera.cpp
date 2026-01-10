#include "raylib.h"

int main() {
    const int screenWidth  = 2048;
    const int screenHeight = 1152;

    InitWindow(screenWidth, screenHeight, "Raylib Scrollable Example");
    SetTargetFPS(120); // Matching your laptop specs/preference

    // --- 1. SETUP CAMERA ---
    Camera2D camera = {0};
    camera.target   = {0, 0}; // Where the camera is looking at inside the world
    camera.offset   = {0, 0}; // Where that point is on the screen (top-left)
    camera.rotation = 0.0f;
    camera.zoom     = 1.0f;

    // Define Colors
    const Color bg        = {164, 195, 178, 255};
    const Color rectColor = {52, 77, 68, 255};

    while (!WindowShouldClose()) {

        // --- 2. UPDATE SCROLL LOGIC ---
        // GetMouseWheelMove() returns typically 1.0 or -1.0
        float wheelMove = GetMouseWheelMove();

        // Move the camera target Y position based on wheel input
        // Multiplied by -20.0f for speed (Negative because scrolling down usually moves view down)
        if (wheelMove != 0) {
            camera.target.y -= (wheelMove * 40.0f);
        }

        // // Optional: Clamp camera so you can't scroll infinitely up into the void
        // if (camera.target.y < 0)
        //     camera.target.y = 0;
        // // Optional: Clamp bottom (assuming content height is roughly 2000px)
        // if (camera.target.y > 1000)
        //     camera.target.y = 1000;

        BeginDrawing();
        ClearBackground(bg);

        // --- 3. BEGIN CAMERA MODE ---
        // Everything between BeginMode2D and EndMode2D will move/scroll
        BeginMode2D(camera);

        // Draw a tall column of rectangles to test scrolling
        for (int i = 0; i < 20; i++) {
            DrawRectangle(100, 100 + (i * 150), 400, 100, rectColor);
            DrawText(TextFormat("Item %d", i), 120, 130 + (i * 150), 40, LIGHTGRAY);
        }

        // Draw something really far down
        DrawText("You scrolled to the bottom!", 100, 3100, 50, RED);

        EndMode2D();
        // --- END CAMERA MODE ---

        // --- 4. STATIC UI (Does not scroll) ---
        // Anything drawn here stays fixed on the screen
        DrawRectangle(0, 0, screenWidth, 80, Fade(BLACK, 1.5f));
        DrawText("Fixed Header - Scroll with Mouse Wheel", 20, 20, 40, WHITE);
        DrawFPS(screenWidth - 100, 20);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
