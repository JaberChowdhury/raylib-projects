#include "CircleShape.h"
#include <cmath>
#include <cstdlib>

CircleShape::CircleShape(float x, float y, float r, float speedX, float speedY) {
    position = {x, y};
    radius   = r;
    velocity = {speedX, speedY};
    mass     = r * r;  // Mass proportional to area (r^2)
    justCollided = false;
    lastCollisionPoint = {0, 0};
    trailLength = 30;  // Number of trail points to store
    cachedSpeed = 0.0f;
    cachedColor = {0, 0, 0, 255};
    speedCacheValid = false;
    colorCacheValid = false;
    
    // Reserve trail capacity to avoid reallocations
    trail.reserve(trailLength + 1);
    
    // Initialize trail with current position
    TrailPoint initialPoint;
    initialPoint.position = position;
    initialPoint.age = 0.0f;
    trail.push_back(initialPoint);
}

void CircleShape::draw(Color color) {
    DrawCircleV(position, radius, color);
}

void CircleShape::drawTrail(Color baseColor) {
    const size_t trailSize = trail.size();
    if (trailSize < 2) return;
    
    // Draw trail as connected circles with fading opacity
    const float radiusScale = 0.7f;
    const float maxAlpha = 150.0f;
    const float lineAlphaScale = 100.0f;
    
    for (size_t i = 0; i < trailSize - 1; i++) {
        const float alpha = 1.0f - trail[i].age;  // Fade from 1.0 to 0.0
        if (alpha <= 0.0f) continue;  // Skip fully faded points
        
        // Calculate trail point size (smaller as it fades)
        float trailRadius = radius * (1.0f - trail[i].age * radiusScale);
        if (trailRadius < 1.0f) trailRadius = 1.0f;
        
        // Create color with fading alpha (reuse same struct)
        Color trailColor = {
            baseColor.r,
            baseColor.g,
            baseColor.b,
            (unsigned char)(alpha * maxAlpha)
        };
        
        // Draw trail point
        DrawCircleV(trail[i].position, trailRadius, trailColor);
        
        // Draw connecting line between trail points for smoother trail
        const float nextAlpha = 1.0f - trail[i + 1].age;
        if (nextAlpha > 0.0f) {
            Color lineColor = {
                baseColor.r,
                baseColor.g,
                baseColor.b,
                (unsigned char)((alpha + nextAlpha) * 0.5f * lineAlphaScale)
            };
            
            DrawLineEx(trail[i].position, trail[i + 1].position, 1.5f, lineColor);
        }
    }
}

void CircleShape::update(int screenWidth, int screenHeight) {
    // Reset collision flag at start of frame
    justCollided = false;
    invalidateCache();  // Invalidate cache when position/velocity changes
    
    // Add current position to trail before moving (use push_front equivalent)
    TrailPoint newPoint;
    newPoint.position = position;
    newPoint.age = 0.0f;
    trail.insert(trail.begin(), newPoint);
    
    // Limit trail length (more efficient than checking every time)
    if (trail.size() > (size_t)trailLength) {
        trail.resize(trailLength);
    }
    
    // Age all trail points (increase age for fading effect)
    const float ageIncrement = 1.0f / trailLength;
    for (auto& point : trail) {
        point.age += ageIncrement;
        if (point.age > 1.0f) {
            point.age = 1.0f;
        }
    }
    
    position.x += velocity.x;
    position.y += velocity.y;

    // Boundary collision with position clamping
    if (position.x - radius <= 0) {
        position.x = radius;
        velocity.x *= -1;
    } else if (position.x + radius >= screenWidth) {
        position.x = screenWidth - radius;
        velocity.x *= -1;
    }
    
    if (position.y - radius <= 0) {
        position.y = radius;
        velocity.y *= -1;
    } else if (position.y + radius >= screenHeight) {
        position.y = screenHeight - radius;
        velocity.y *= -1;
    }
}

bool CircleShape::checkCollision(const CircleShape& other) const {
    float dx = position.x - other.position.x;
    float dy = position.y - other.position.y;
    float distanceSquared = dx * dx + dy * dy;
    float minDistance = radius + other.radius;
    float minDistanceSquared = minDistance * minDistance;
    // Use squared distance to avoid expensive sqrt
    return distanceSquared < minDistanceSquared;
}

Vector2 CircleShape::getCollisionPoint(const CircleShape& other) const {
    // Calculate the point of collision (midpoint between centers)
    float dx = other.position.x - position.x;
    float dy = other.position.y - position.y;
    float distance = sqrtf(dx * dx + dy * dy);
    
    if (distance == 0.0f) {
        return position;
    }
    
    // Return point along the line between centers, closer to this particle
    float ratio = radius / (radius + other.radius);
    return {
        position.x + dx * ratio,
        position.y + dy * ratio
    };
}

void CircleShape::resolveCollision(CircleShape& other) {
    // Mark both particles as having collided
    justCollided = true;
    other.justCollided = true;
    
    // Calculate collision normal (vector from this to other)
    Vector2 normal = {
        other.position.x - position.x,
        other.position.y - position.y
    };
    
    float distanceSquared = normal.x * normal.x + normal.y * normal.y;
    float distance = sqrtf(distanceSquared);
    
    // Handle edge case where particles are exactly on top of each other
    if (distance < 0.0001f) {
        // Push them apart in a random direction
        float angle = (float)(rand() % 360) * 3.14159f / 180.0f;
        normal.x = cosf(angle);
        normal.y = sinf(angle);
        distance = 0.0001f;
    } else {
        normal.x /= distance;
        normal.y /= distance;
    }
    
    // Store collision point for visual feedback
    float ratio = radius / (radius + other.radius);
    lastCollisionPoint = {
        position.x + normal.x * radius,
        position.y + normal.y * radius
    };
    other.lastCollisionPoint = {
        other.position.x - normal.x * other.radius,
        other.position.y - normal.y * other.radius
    };
    
    // Separate circles to prevent overlap (CRITICAL - must happen first)
    float overlap = (radius + other.radius) - distance;
    if (overlap > 0.0f) {
        // Separate based on inverse mass ratio (lighter objects move more)
        float totalMass = mass + other.mass;
        if (totalMass > 0.0f) {
            float separationRatio1 = other.mass / totalMass;
            float separationRatio2 = mass / totalMass;
            
            // Move particles apart along the collision normal
            float separationX = normal.x * overlap;
            float separationY = normal.y * overlap;
            
            position.x -= separationX * separationRatio1;
            position.y -= separationY * separationRatio1;
            other.position.x += separationX * separationRatio2;
            other.position.y += separationY * separationRatio2;
        }
    }
    
    // Calculate relative velocity
    Vector2 relativeVelocity = {
        velocity.x - other.velocity.x,
        velocity.y - other.velocity.y
    };
    
    // Calculate relative velocity along collision normal
    float velocityAlongNormal = relativeVelocity.x * normal.x + relativeVelocity.y * normal.y;
    
    // Only resolve if particles are moving towards each other
    // (negative means approaching, positive means separating)
    if (velocityAlongNormal > 0.0f) {
        return;  // Already separating, no need to resolve
    }
    
    // Calculate restitution (bounciness) - 0.95 for nearly elastic collisions
    float restitution = 0.95f;
    
    // Calculate impulse scalar using conservation of momentum
    // Formula: j = -(1 + e) * v_rel · n / (1/m1 + 1/m2)
    float impulseScalar = -(1.0f + restitution) * velocityAlongNormal;
    float invMass1 = 1.0f / mass;
    float invMass2 = 1.0f / other.mass;
    impulseScalar /= (invMass1 + invMass2);
    
    // Apply impulse to both particles
    Vector2 impulse = {
        impulseScalar * normal.x,
        impulseScalar * normal.y
    };
    
    // Update velocities (impulse = change in momentum)
    velocity.x += impulse.x * invMass1;
    velocity.y += impulse.y * invMass1;
    other.velocity.x -= impulse.x * invMass2;
    other.velocity.y -= impulse.y * invMass2;
    
    // Invalidate cache after collision
    invalidateCache();
    other.invalidateCache();
}

float CircleShape::getSpeed() const {
    if (!speedCacheValid) {
        cachedSpeed = sqrtf(velocity.x * velocity.x + velocity.y * velocity.y);
        speedCacheValid = true;
    }
    return cachedSpeed;
}

void CircleShape::invalidateCache() {
    speedCacheValid = false;
    colorCacheValid = false;
    cachedSpeed = 0.0f;
}
