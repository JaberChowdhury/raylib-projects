#include "CircleShape.h"

CircleShape::CircleShape(float x, float y, float r, float speedX, float speedY) {
    position = {x, y};
    radius   = r;
    velocity = {speedX, speedY};
}

void CircleShape::draw(Color color) {
    DrawCircleV(position, radius, color);
}

void CircleShape::update(int screenWidth, int screenHeight) {
    position.x += velocity.x;
    position.y += velocity.y;

    if ((position.x + radius >= screenWidth) || (position.x - radius <= 0)) {
        velocity.x *= -1;
    }
    if ((position.y + radius >= screenHeight) || (position.y - radius <= 0)) {
        velocity.y *= -1;
    }
}
