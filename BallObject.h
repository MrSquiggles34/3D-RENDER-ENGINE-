#ifndef BALLOBJECT_H
#define BALLOBJECT_H

#include "GameObject.h"

class BallObject : public GameObject
{
public:
    // ball state	
    float     Radius;

    BallObject();
    BallObject(glm::vec3 pos, float radius, glm::vec3 velocity, Texture2D sprite);

    glm::vec2 Move(float dt);
    void      Reset(glm::vec3 position, glm::vec3 velocity);

};

#endif // BALL OBJECT