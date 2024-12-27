
#ifndef GAME_H
#define GAME_H

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"


#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <camera.h>
#include <iostream>
#include <tuple>
#include <vector>
#include <atomic>

#include "BallObject.h"
#include "WallObject.h"
#include "Node.h"


// Represents the current state of the game
enum GameState {
    GAME_ACTIVE,
};



// Game-related State data


const glm::vec3 INITIAL_BALL_VELOCITY(0.1f, 0.0f, 0.1f);
const float BALL_RADIUS = 0.5f;

// Game holds all game-related state and functionality.
// Combines all game-related data into a single class for
// easy access to each of the components and manageability.
class Game
{
    public:
        // game state
        GameState               State;
        Camera                  camera;
        bool                    Keys[1024];
        unsigned int            Width, Height;

        SpriteRenderer* Renderer;

        std::vector<BallObject*> _balls; //All of the balls in play
        std::vector<WallObject*> Walls;

        // Multithreadding
        std::mutex collisionMutex;
        std::atomic<int*> activeThreadCount{ 0 };
        std::atomic<int*> totalThreadsLaunched{ 0 };

        int* getTotalThreadsLaunched() const {
            return totalThreadsLaunched;
        }

        int* getActiveThreadCount() const {
            return activeThreadCount;
        }

        // constructor/destructor
        Game(unsigned int width, unsigned int height);
        ~Game();
        // initialize game state (load all shaders/textures/levels)
        void Init();
        // game loop
        void ProcessInput(float dt);
        void Update(float dt);
        void Render();
        void ConstructQuadTree(Node* root, float dt);

        // Multi threading
        void MultithreadedUpdateMovement(float dt, int startIdx, int endIdx);

        float randomFloat()
        {
            return (float)(rand()) / (float)(rand());
        }

        // Generate random vector
        glm::vec3 generateRandom3DVector() {
            // Create random number generator
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_real_distribution<float> dis(-4.0f, 4.0f);

            // Generate random components
            float x = dis(gen);
            float y = 0.0f;
            float z = dis(gen);

            return { x, y, z };
        }

        void DetectCollisions()
        {
            // Brute force collision detection: compare each ball with every other ball
            for (size_t i = 0; i < _balls.size(); ++i)
            {
                for (size_t j = i + 1; j < _balls.size(); ++j)
                {
                    BallObject* ballA = _balls[i];
                    BallObject* ballB = _balls[j];

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

};

// GLFW function declarations
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

// For the camera
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

#endif