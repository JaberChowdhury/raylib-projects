#include "CircleShape.h"
#include "includes/GenerateMaterialColor.h"
#include "raylib.h"
#include <vector>
#include <cmath>
#include <algorithm>

// 1. Define Constants globally so the Class can see them
const int SCREEN_WIDTH  = 2048;
const int SCREEN_HEIGHT = 1152;

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

    // --- CREATE PARTICLES ---
    std::vector<CircleShape> circles;

    // Loop to add more circles for better collision animation
    int numParticles = 500;
    for (int i = 0; i < numParticles; i++) {
        float r  = GetRandomValue(5, 15);
        float x  = GetRandomValue(r, SCREEN_WIDTH - r);
        float y  = GetRandomValue(r, SCREEN_HEIGHT - r);
        float sx = GetRandomValue(-8, 8);
        float sy = GetRandomValue(-8, 8);

        // Create the circle and add it to the list
        circles.push_back(CircleShape(x, y, r, sx, sy));
    }
    // Store collision pairs for visual feedback (use set for O(1) lookup)
    std::vector<std::pair<int, int>> collisionPairs;
    collisionPairs.reserve(50);  // Reserve space to avoid reallocations
    
    const size_t numCircles = circles.size();
    
    while (!WindowShouldClose()) {
        // Clear collision pairs from last frame
        collisionPairs.clear();
        
        // Check and resolve collisions BEFORE updating positions
        // This prevents particles from passing through each other
        for (size_t i = 0; i < numCircles; i++) {
            for (size_t j = i + 1; j < numCircles; j++) {
                if (circles[i].checkCollision(circles[j])) {
                    circles[i].resolveCollision(circles[j]);
                    collisionPairs.push_back({(int)i, (int)j});
                }
            }
        }
        
        // Update physics (move particles) AFTER collision resolution
        for (size_t i = 0; i < numCircles; i++) {
            circles[i].update(SCREEN_WIDTH, SCREEN_HEIGHT);
        }
        
        // Additional collision check after movement to catch any missed collisions
        // Use a simple approach - just check and add if not already present
        for (size_t i = 0; i < numCircles; i++) {
            for (size_t j = i + 1; j < numCircles; j++) {
                if (circles[i].checkCollision(circles[j])) {
                    circles[i].resolveCollision(circles[j]);
                    // Check if pair already exists (simple linear search, but small set)
                    bool found = false;
                    for (const auto& pair : collisionPairs) {
                        if ((pair.first == (int)i && pair.second == (int)j) || 
                            (pair.first == (int)j && pair.second == (int)i)) {
                            found = true;
                            break;
                        }
                    }
                    if (!found) {
                        collisionPairs.push_back({(int)i, (int)j});
                    }
                }
            }
        }

        // Draw
        BeginDrawing();
        ClearBackground(ctx.bg);

        // Draw collision lines first (behind particles)
        for (const auto& pair : collisionPairs) {
            int i = pair.first;
            int j = pair.second;
            // Draw a line between colliding particles
            DrawLineEx(
                circles[i].lastCollisionPoint,
                circles[j].lastCollisionPoint,
                2.0f,
                (Color){255, 255, 255, 100}  // White semi-transparent line
            );
        }

        // Draw trails first (behind particles) - cache color calculations
        for (size_t i = 0; i < numCircles; i++) {
            // Calculate speed and color once
            float speed = circles[i].getSpeed();
            
            // Map speed to hue (0-360) - faster particles get different colors
            int hue = (int)(speed * 10.0f) % 360;
            int shade = 500 + (int)(speed * 5.0f);
            if (shade > 900) shade = 900;
            if (shade < 50) shade = 50;
            
            Color trailBaseColor = GenerateMaterialColor(hue, shade);
            circles[i].cachedColor = trailBaseColor;  // Cache for particle drawing
            circles[i].colorCacheValid = true;
            circles[i].drawTrail(trailBaseColor);
        }

        // Draw particles with varying colors based on velocity for visual feedback
        for (size_t i = 0; i < numCircles; i++) {
            // Use cached color if available, otherwise calculate
            Color particleColor;
            if (circles[i].colorCacheValid) {
                particleColor = circles[i].cachedColor;
            } else {
                float speed = circles[i].getSpeed();
                int hue = (int)(speed * 10.0f) % 360;
                int shade = 500 + (int)(speed * 5.0f);
                if (shade > 900) shade = 900;
                if (shade < 50) shade = 50;
                particleColor = GenerateMaterialColor(hue, shade);
                circles[i].cachedColor = particleColor;
                circles[i].colorCacheValid = true;
            }
            
            // Make colliding particles brighter/more visible
            if (circles[i].justCollided) {
                const float brightnessBoost = 1.3f;
                particleColor.r = (unsigned char)(std::min(255.0f, particleColor.r * brightnessBoost));
                particleColor.g = (unsigned char)(std::min(255.0f, particleColor.g * brightnessBoost));
                particleColor.b = (unsigned char)(std::min(255.0f, particleColor.b * brightnessBoost));
            }
            
            // Draw particle with glow effect (stronger for colliding particles)
            const unsigned char glowAlpha = circles[i].justCollided ? 100u : 50u;
            const Color glowColor = {
                particleColor.r,
                particleColor.g,
                particleColor.b,
                glowAlpha
            };
            DrawCircleV(circles[i].position, circles[i].radius + 3, glowColor);
            
            // Draw the main particle
            circles[i].draw(particleColor);
            
            // Draw collision point indicator
            if (circles[i].justCollided) {
                DrawCircleV(circles[i].lastCollisionPoint, 3.0f, 
                           (Color){255, 255, 255, 200});
            }
        }

        DrawFPS(12, 12);
        DrawTextCentered("Particle Collision Animation by ai", 50, 60, 10, ctx);

        EndDrawing();
    }

    UnloadFont(ctx.myfont);
    CloseWindow();
    return 0;
}
