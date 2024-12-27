// WallObject.h
#ifndef WALLOBJECT_H
#define WALLOBJECT_H

#include "GameObject.h"

class WallObject : public GameObject
{
public:
    // Default Constructor
    WallObject();

    // Parameterized Constructor
    WallObject(glm::vec3 pos, glm::vec3 size, Texture2D sprite);
};

#endif // WALLOBJECT_H