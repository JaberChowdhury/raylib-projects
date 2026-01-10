#include "raylib.h"
#include <math.h> // Required for sin()

int main() {
    const int screenWidth  = 2048;
    const int screenHeight = 1152;

    // Use your preferred color palette
    const Color bg        = {164, 195, 178, 255};
    const Color waveColor = {52, 77, 68, 255};

    InitWindow(screenWidth, screenHeight, "Raylib Sine Wave Animation");
    SetTargetFPS(120);

    // Animation variables
    float time = 0.0f;

    // Wave settings
    float amplitude = 200.0f; // How tall the wave is
    float frequency = 0.005f; // How "tight" the wave is (lower = wider)
    float speed     = 0.05f;  // How fast it moves

    while (!WindowShouldClose()) {
        // 1. Update
        // Increment time to make the wave move
        time += speed;

        // 2. Draw
        BeginDrawing();
        ClearBackground(bg);

        // We draw the wave by connecting small lines
        int step = 1; // Resolution: lower is smoother but heavier

        Vector2 prevPoint;
        bool    firstPoint = true;

        for (int x = 0; x < screenWidth; x += step) {
            // MATH: y = CenterY + sin(x * freq + time) * amp
            float y = (screenHeight / 2.0f) + sinf((x * frequency) + time) * amplitude;

            Vector2 currentPoint = {(float) x, y};

            if (!firstPoint) {
                // Draw line from the previous point to the current point
                DrawLineEx(prevPoint, currentPoint, 5.0f, waveColor);
            } else {
                firstPoint = false;
            }

            prevPoint = currentPoint;
        }

        // Draw some text info
        DrawText("y = sin(x + time)", 50, 50, 40, waveColor);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
