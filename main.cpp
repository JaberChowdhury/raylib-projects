#include "raylib.h"
#include <cstddef>
#include <ctime>
#include <iostream>

#define WIDTH 2340
#define HIGHT 1800
#define particle_count 32

Color GenerateMaterialColor(int hue, int shade) {
    // Validate hue
    if (hue < 0 || hue > 360) {
        // Return black or a default error color
        return (Color) {0, 0, 0, 255};
    }

    // Calculate "Lightness" (Acts as Value in HSV model based on your logic)
    double value = 1.0 - (shade / 1000.0);

    // Material Design often keeps the 500 shade as the base mid-tone
    if (shade == 500)
        value = 0.5;

    // Saturation is fixed at 0.5 in your logic
    double saturation = 0.5;

    // HSV to RGB Conversion
    int h_sector = hue / 60;
    // Handle hue 360 as 0
    if (h_sector >= 6)
        h_sector = 0;

    // Calculate fractional part of hue sector
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

class Particle {
  public:
    float x, y, vx, vy, r;
    Color color;
    Particle(float x, float y, float vx, float vy, float r, Color color) {
        this->x     = x;
        this->y     = y;
        this->vx    = vx;
        this->vy    = vy;
        this->r     = r;
        this->color = color;

        // std::cout << "Particle is created" << std::endl;
    }
    void draw() { DrawCircle(this->x, this->y, this->r, this->color); }
    void update() {

        if ((this->x - this->r) < 0) {
            this->vx = -this->vx;
        }
        if ((this->x + this->r) > WIDTH) {
            this->vx = -this->vx;
        }
        if ((this->y - this->r) < 0) {
            this->vy = -this->vy;
        }
        if ((this->y + this->r) > HIGHT) {
            this->vy = -this->vy;
        }
        this->x += this->vx;
        this->y += this->vy;
    }
    ~Particle() {
        std::cout << "Particle is deleted" << std::endl;
        delete this;
    }
};

int main() {

    InitWindow(WIDTH, HIGHT, "My ossam!! window");

    SetTargetFPS(60);

    Particle* particles[particle_count];
    Particle* new_particle;
    int       shades[]    = {50, 100, 200, 300, 400, 500, 600, 700, 800, 900};
    int       shadesCount = sizeof(shades) / sizeof(shades[0]);
    SetRandomSeed(std::time(NULL));
    for (int i = 0; i < particle_count; i++) {
        int radius   = GetRandomValue(30, 50);
        new_particle = new Particle(
            //
            GetRandomValue(radius, WIDTH - radius), // pos x
            //
            GetRandomValue(radius, HIGHT - radius), // pos y
            //
            GetRandomValue(-15, 15), // vx
            //
            GetRandomValue(-15, 15), // vy
            //
            radius,
            //
            GenerateMaterialColor(i, shades[GetRandomValue(0, (shadesCount - 1))]));
        particles[i] = new_particle;
    }

    while (!WindowShouldClose()) {
        BeginDrawing();

        ClearBackground(WHITE);
        DrawFPS(10, 10);
        for (int i = 0; i < particle_count; i++) {
            particles[i]->draw();
            particles[i]->update();
        }

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
