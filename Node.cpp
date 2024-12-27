#include "Node.h"

int MAX_LVL = 4;
std::queue<Node*> Node::NodePool;

Node* Node::Create(int _x, int _y, int _w, int _h, int _lvl) {
    if (!NodePool.empty()) {
        Node* node = NodePool.front();
        NodePool.pop();
        node->Set(_x, _y, _w, _h, _lvl);
        return node;
    }
    return new Node(_x, _y, _w, _h, _lvl);
}

void Node::Add(BallObject* _entity) {
    if (entities.size() < 4 || lvl == MAX_LVL) {
        entities.push_back(_entity);
    }
    else {
        if (children.empty()) {
            for (int i = 0; i < 4; ++i) {
                Node* child = Create(0, 0, 0, 0, lvl + 1);
                children.push_back(child);
            }
            children[0]->Set(x, y, w / 2, h / 2, lvl + 1);
            children[1]->Set(x + w / 2, y, w / 2, h / 2, lvl + 1);
            children[2]->Set(x, y + h / 2, w / 2, h / 2, lvl + 1);
            children[3]->Set(x + w / 2, y + h / 2, w / 2, h / 2, lvl + 1);
        }

        bool added = false;
        for (auto* child : children) {
            if (child->Inside(_entity)) {
                child->Add(_entity);
                added = true;
                break; // Once added to a child, stop checking further
            }
        }

        if (!added) {
            entities.push_back(_entity); // If the entity doesn't fit into any child, keep it in the current node
        }
    }
}


int Node::Inside(BallObject* _entity) {
    float px0 = _entity->Position.x - _entity->Radius;
    float py0 = _entity->Position.y - _entity->Radius;
    float px1 = _entity->Position.x + _entity->Radius;
    float py1 = _entity->Position.y + _entity->Radius;

    return (px0 >= x && px0 <= x + w && py0 >= y && py0 <= y + h) ||
        (px1 >= x && px1 <= x + w && py1 >= y && py1 <= y + h);
}

void Node::Set(int _x, int _y, int _w, int _h, int _lvl) {
    for (auto* child : children) {
        NodePool.push(child);
    }
    children.clear();
    entities.clear();

    x = _x;
    y = _y;
    w = _w;
    h = _h;
    lvl = _lvl;
}


void Node::DetectCollisions()
{
    // Step 1: Detect collisions among entities in the current node
    for (size_t i = 0; i < entities.size(); ++i)
    {
        for (size_t j = i + 1; j < entities.size(); ++j)
        {
            BallObject* ballA = entities[i];
            BallObject* ballB = entities[j];

            // Perform collision detection and resolution
            glm::vec3 difference = ballB->Position - ballA->Position;
            float distance = glm::length(difference);
            float radiusSum = ballA->Radius * 2 + ballB->Radius * 2;

            if (distance < radiusSum)
            {
                // Normalize the collision normal
                glm::vec3 collisionNormal = glm::normalize(difference);

                // Resolve overlap
                float penetrationDepth = radiusSum - distance;
                glm::vec3 correction = collisionNormal * (penetrationDepth / 2.0f);
                ballA->Position -= correction;
                ballB->Position += correction;

                // Calculate relative velocity
                glm::vec3 relativeVelocity = ballB->Velocity - ballA->Velocity;
                float velocityAlongNormal = glm::dot(relativeVelocity, collisionNormal);

                if (velocityAlongNormal > 0.0f) continue;

                // Calculate impulse
                float impulseScalar = -(1.0f) * velocityAlongNormal;
                impulseScalar /= (1.0f / ballA->Radius) + (1.0f / ballB->Radius);

                glm::vec3 impulse = impulseScalar * collisionNormal;
                ballA->Velocity -= impulse / ballA->Radius;
                ballB->Velocity += impulse / ballB->Radius;
            }
        }
    }

    // Step 2: Check for collisions recursively in child nodes
    for (Node* child : children)
    {
        child->DetectCollisions();
    }

    // Step 3: Check for collisions between entities in this node and entities in child nodes
    for (size_t i = 0; i < entities.size(); ++i)
    {
        for (Node* child : children)
        {
            for (BallObject* childEntity : child->entities)
            {
                BallObject* ballA = entities[i];
                BallObject* ballB = childEntity;

                // Perform collision detection and resolution
                glm::vec3 difference = ballB->Position - ballA->Position;
                float distance = glm::length(difference);
                float radiusSum = ballA->Radius * 2 + ballB->Radius * 2;

                if (distance < radiusSum)
                {
                    // Normalize the collision normal
                    glm::vec3 collisionNormal = glm::normalize(difference);

                    // Resolve overlap
                    float penetrationDepth = radiusSum - distance;
                    glm::vec3 correction = collisionNormal * (penetrationDepth / 2.0f);
                    ballA->Position -= correction;
                    ballB->Position += correction;

                    // Calculate relative velocity
                    glm::vec3 relativeVelocity = ballB->Velocity - ballA->Velocity;
                    float velocityAlongNormal = glm::dot(relativeVelocity, collisionNormal);

                    if (velocityAlongNormal > 0.0f) continue;

                    // Calculate impulse
                    float impulseScalar = -(1.0f) * velocityAlongNormal;
                    impulseScalar /= (1.0f / ballA->Radius) + (1.0f / ballB->Radius);

                    glm::vec3 impulse = impulseScalar * collisionNormal;
                    ballA->Velocity -= impulse / ballA->Radius;
                    ballB->Velocity += impulse / ballB->Radius;
                }
            }
        }
    }
}
