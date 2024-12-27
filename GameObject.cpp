#include "GameObject.h"


GameObject::GameObject()
    : Position(0.0f, 0.0f, 0.0f), Size(1.0f, 1.0f, 1.0f), Velocity(0.0f), Color(1.0f), Rotation(0.0f), Acceleration(0.0f, 0.0f, 0.0f), Sprite(), IsSphere(false), Destroyed(false) { }

GameObject::GameObject(glm::vec3 pos, glm::vec3 size, Texture2D sprite, glm::vec3 color, glm::vec3 velocity, glm::vec3 rotation, glm::vec3 acceleration)
    : Position(pos), Size(size), Velocity(velocity), Color(color), Rotation(rotation), Acceleration(acceleration), Sprite(sprite), IsSphere(false), Destroyed(false){ }

void GameObject::Draw(SpriteRenderer& renderer)
{
    renderer.DrawSprite(this->Sprite, this->Position, this->Size, this->Rotation, this->Color, this->IsSphere);
}