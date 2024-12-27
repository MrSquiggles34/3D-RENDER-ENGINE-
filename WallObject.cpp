// WallObject.cpp
#include "WallObject.h"

// Default Constructor
WallObject::WallObject()
    : GameObject() { }

// Parameterized Constructor
WallObject::WallObject(glm::vec3 pos, glm::vec3 size, Texture2D sprite)
    : GameObject(pos, size, sprite, glm::vec3(1.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.0f)) { }
