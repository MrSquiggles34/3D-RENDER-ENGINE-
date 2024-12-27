#include "BallObject.h"

BallObject::BallObject()
    : GameObject(), Radius(1.0f) { }

BallObject::BallObject(glm::vec3 pos, float radius, glm::vec3 velocity, Texture2D sprite)
    : GameObject(pos, glm::vec3(radius * 2.0f, radius * 2.0f, radius * 2.0f), sprite, glm::vec3(1.0f), velocity), Radius(radius){ }

glm::vec2 BallObject::Move(float dt)
{
    // Update velocity based on acceleration
    this->Velocity += this->Acceleration * dt;

    if (Position.x >= 45.0f || Position.x <= -45.0f) {
        Velocity.x = Velocity.x * -1.0;
    }
    if (Position.z >= 45.0f || Position.z <= -45.0f) {
        Velocity.z = Velocity.z * -1.0;
    }
    
    if (Position.y < 2.0f && fabs(Velocity.y) < 0.1f) {
        Velocity.y = 0.0f; // Stop bouncing if velocity is negligible
    }
    
    // Handle bounce with floor
    if (Position.y < 2.0f) {
        Position.y = 2.0f; // Ensure it stays above the floor
        if (fabs(Velocity.y) < 0.1f) {
            Velocity.y = 0.0f; // Stop if too slow
        }
        else {
            Velocity.y = -Velocity.y * 0.8f; // Regular bounce with restitution
        }
    }

    // Update position based on velocity
    this->Position += this->Velocity * dt;

    return this->Position;
}

// resets the ball to initial Stuck Position (if ball is outside window bounds)
void BallObject::Reset(glm::vec3 position, glm::vec3 velocity)
{
    this->Position = position;
    this->Velocity = velocity;
}