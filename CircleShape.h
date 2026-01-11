#pragma once
#include "raylib.h"

class CircleShape {
  public:
    Vector2 position;
    Vector2 velocity;
    float   radius;

    CircleShape(float x, float y, float r, float speedX, float speedY);
    void draw(Color color);
    void update(int screenWidth, int screenHeight);
};
