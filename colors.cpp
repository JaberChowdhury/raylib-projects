#include "raylib.h"
#include <string>
#include <vector>

const int screenWidth  = 2048;
const int screenHeight = 1152;

// Helper to draw centered text
void DrawTextCentered(Font font, const char* text, float y, float fontSize, float spacing, Color color) {
    Vector2 textSize = MeasureTextEx(font, text, fontSize, spacing);
    float   x        = (screenWidth - textSize.x) / 2.0f;
    DrawTextEx(font, text, {x, y}, fontSize, spacing, color);
}

// Function to generate a Texture containing all RGB colors
// We create an Image, write pixels directly to memory, then upload to GPU.
Texture2D GenerateRGBTexture() {
    int width  = 4096; // Sqrt(256*256*256) is 4096. This fits all colors in a square.
    int height = 4096;

    // Allocate an array of Colors (using Raylib's Color struct)
    Color* pixels = (Color*) malloc(width * height * sizeof(Color));

    int index = 0;
    // Your requested loop order
    for (int r = 255; r >= 0; r--) {
        for (int g = 255; g >= 0; g--) {
            for (int b = 255; b >= 0; b--) {
                if (index < width * height) {
                    pixels[index] = (Color) {(unsigned char) r, (unsigned char) g, (unsigned char) b, 255};
                    index++;
                }
            }
        }
    }

    // Create Raylib Image from raw data
    Image img = {
        .data = pixels, .width = width, .height = height, .mipmaps = 1, .format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8};

    // Upload to GPU
    Texture2D texture = LoadTextureFromImage(img);

    // Free CPU memory (we only need the GPU texture now)
    UnloadImage(img);

    return texture;
}

int main() {
    InitWindow(screenWidth, screenHeight, "Raylib RGB Spectrum");
    SetTargetFPS(120);

    // Define Colors
    const Color bg        = {164, 195, 178, 255};
    const Color textColor = {52, 77, 68, 255};

    ChangeDirectory(GetApplicationDirectory());
    Font myfont = LoadFontEx("resources/zh-cn.ttf", 96, 0, 0);
    SetTextureFilter(myfont.texture, TEXTURE_FILTER_BILINEAR);

    // --- GENERATE COLORS ONCE (Before Loop) ---
    // This might take 1-2 seconds on startup
    Texture2D colorTexture = GenerateRGBTexture();

    // Camera Setup
    Camera2D cam = {0};
    cam.zoom     = 1.0f;
    cam.target   = {0, 0};
    cam.offset   = {0, 0}; // Center of screen? Or top left.

    float trackPosY = 0;

    while (!WindowShouldClose()) {
        // --- Input Logic ---
        float wheelmove = GetMouseWheelMove();

        // Scroll Camera
        if (wheelmove != 0) {
            cam.target.y -= (wheelmove * 60); // Faster scroll speed
        }

        // Pan Camera with Right Mouse Button (Optional, helpful for large textures)
        if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
            Vector2 delta = GetMouseDelta();
            cam.target.x -= delta.x;
            cam.target.y -= delta.y;
        }

        // --- Drawing ---
        BeginDrawing();
        ClearBackground(bg);

        BeginMode2D(cam);

        trackPosY = 50;
        DrawTextCentered(myfont, "RGB Color Space (16.7 Million Colors)", trackPosY, 80, 1, textColor);

        trackPosY = 200;

        // Draw the generated texture
        // We draw it centered horizontally relative to our coordinate system
        float textureX = (screenWidth - colorTexture.width) / 2.0f;
        DrawTexture(colorTexture, textureX, trackPosY, WHITE);

        // Draw a border around it
        DrawRectangleLines(textureX, trackPosY, colorTexture.width, colorTexture.height, textColor);

        // Draw text below
        trackPosY += colorTexture.height + 50;
        DrawTextCentered(myfont, "End of Spectrum", trackPosY, 60, 1, textColor);

        EndMode2D();

        // Static UI (FPS)
        DrawFPS(10, 10);

        EndDrawing();
    }

    // Cleanup
    UnloadTexture(colorTexture);
    UnloadFont(myfont);
    CloseWindow();

    return 0;
}
