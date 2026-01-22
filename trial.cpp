#include "raylib.h"
#include <cmath>
#include <iostream>
#include <vector>

// Function to generate a single Material Color based on hue and shade
// Returns a single Raylib Color struct
Color GenerateMaterialColor(int hue, int shade) {
    // Validate hue
    if (hue < 0 || hue > 360) {
        return (Color) {0, 0, 0, 255};
    }

    // Calculate "Lightness"
    double value = 1.0 - (shade / 1000.0);
    if (shade == 500)
        value = 0.5;

    double saturation = 0.5;

    // HSV to RGB Conversion
    int h_sector = hue / 60;
    if (h_sector >= 6)
        h_sector = 0;

    double f = (hue / 60.0) - (int) (hue / 60.0);
    double p = value * (1 - saturation);
    double q = value * (1 - f * saturation);
    double t = value * (1 - (1 - f) * saturation);

    int r = 0, g = 0, b = 0;

    switch (h_sector) {
    case 0:
        r = value * 255;
        g = t * 255;
        b = p * 255;
        break;
    case 1:
        r = q * 255;
        g = value * 255;
        b = p * 255;
        break;
    case 2:
        r = p * 255;
        g = value * 255;
        b = t * 255;
        break;
    case 3:
        r = p * 255;
        g = q * 255;
        b = value * 255;
        break;
    case 4:
        r = t * 255;
        g = p * 255;
        b = value * 255;
        break;
    case 5:
        r = value * 255;
        g = p * 255;
        b = q * 255;
        break;
    default:
        r = value * 255;
        g = p * 255;
        b = q * 255;
        break;
    }

    return (Color) {(unsigned char) r, (unsigned char) g, (unsigned char) b, 255};
}

int main() {
    const int screenWidth  = 2800;
    const int screenHeight = 1600;
    InitWindow(screenWidth, screenHeight, "Raylib - Particle Trails");

    // 1. Create a persistent texture (the "Canvas")
    // We will draw to this instead of the screen directly to avoid flickering
    RenderTexture2D canvas = LoadRenderTexture(screenWidth, screenHeight);

    // Initialize the canvas with a solid color once
    BeginTextureMode(canvas);
    ClearBackground(BLACK);
    EndTextureMode();

    SetTargetFPS(160);

    float hue = 0.0f;

    while (!WindowShouldClose()) {
        // Update
        hue += 1.0f;
        if (hue > 360)
            hue = 0;

        Vector2 mousePos      = GetMousePosition();
        Color   particleColor = GenerateMaterialColor((int) hue, 500);

        // -------------------------------------------------------------------------
        // DRAWING PHASE 1: Draw to the persistent Canvas (RenderTexture)
        // -------------------------------------------------------------------------
        BeginTextureMode(canvas);

        // THE TRAIL TRICK:
        // Instead of clearing the background, draw a semi-transparent black rectangle
        // over the entire canvas. This dims the previous frame's content.
        // Alpha 25 means ~10% opacity (slow fade). Increase alpha for shorter trails.
        DrawRectangle(0, 0, screenWidth, screenHeight, (Color) {0, 0, 0, 25});

        // Draw the circle on top of the "faded" background
        DrawCircleV(mousePos, 30, particleColor);

        EndTextureMode();

        // -------------------------------------------------------------------------
        // DRAWING PHASE 2: Draw the Canvas to the actual Screen
        // -------------------------------------------------------------------------
        BeginDrawing();
        ClearBackground(BLACK);

        // Draw the texture we just updated.
        // NOTE: We must flip the height (negative height) because OpenGL coordinates
        // for textures are inverted compared to Raylib screen coordinates.
        DrawTextureRec(canvas.texture, (Rectangle) {0, 0, (float) canvas.texture.width, (float) -canvas.texture.height},
                       (Vector2) {0, 0}, WHITE);

        DrawText("Move mouse to generate trails", 10, 10, 20, WHITE);
        DrawFPS(10, 40);
        EndDrawing();
    }

    // Cleanup
    UnloadRenderTexture(canvas);
    CloseWindow();
    return 0;
}
