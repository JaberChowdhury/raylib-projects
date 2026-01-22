#pragma once
#include "raylib.h"
#include <vector>

struct TrailPoint {
    Vector2 position;
    float age;  // 0.0 = newest, 1.0 = oldest (faded out)
};

class CircleShape {
  public:
    Vector2 position;
    Vector2 velocity;
    float   radius;
    float   mass;  // Mass for collision physics
    bool    justCollided;  // Flag for visual feedback
    Vector2 lastCollisionPoint;  // For drawing collision lines
    std::vector<TrailPoint> trail;  // Trail of previous positions
    int trailLength;  // Maximum number of trail points
    
    // Cached values to avoid recalculations
    mutable float cachedSpeed;
    mutable Color cachedColor;
    mutable bool speedCacheValid;
    mutable bool colorCacheValid;

    CircleShape(float x, float y, float r, float speedX, float speedY);
    void draw(Color color);
    void drawTrail(Color baseColor);
    void update(int screenWidth, int screenHeight);
    
    // Collision detection and response
    bool checkCollision(const CircleShape& other) const;
    void resolveCollision(CircleShape& other);
    Vector2 getCollisionPoint(const CircleShape& other) const;
    
    // Helper methods for cached calculations
    float getSpeed() const;
    void invalidateCache();
};
