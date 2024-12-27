#include "Game.h"
#include "ResourceManager.h"
#include "SpriteRenderer.h"

Game::Game(unsigned int width, unsigned int height)
    : State(GAME_ACTIVE), Keys(), Width(width), Height(height), camera(glm::vec3(0.0f, 80.0f, 80.0f))
{

}

Game::~Game()
{
    delete Renderer;

    for (auto wall : Walls) {
        delete wall;
    }

    for (auto ball : _balls) {
        delete ball;
    }
}


void Game::Init()
{
    // load shaders
    ResourceManager::LoadShader("shaders/vertexShader.glsl", "shaders/fragmentShader.glsl", nullptr, "sprite");
    ResourceManager::LoadShader("shaders/skyboxVertexShader.glsl", "shaders/skyboxFragmentShader.glsl", nullptr, "skybox");

    // configure shaders
    // pass projection matrix to shader (note that in this case it could change every frame)
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)Width / (float)Height, 0.1f, 1000.0f);

    ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
    ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);

    // set render-specific controls
    Renderer = new SpriteRenderer(ResourceManager::GetShader("sprite"));
    
    // load textures
    ResourceManager::LoadTexture("resources/textures/iron.png", true, "iron");
    ResourceManager::LoadTexture("resources/textures/wood.png", true, "wood");
    ResourceManager::LoadTexture("resources/textures/marble.png", true, "marble");
    ResourceManager::LoadTexture("resources/textures/bmarble.png", true, "bmarble");
    ResourceManager::LoadTexture("resources/textures/gmarble.png", true, "gmarble");

    Walls.push_back(new WallObject(glm::vec3(0.0f), glm::vec3(90.0f, 1.0f, 90.0f), ResourceManager::GetTexture("iron")));

    // Add 20 balls with a random position, velocity, radius, and color
    for (int i = 0; i < 60; i++) {
        glm::vec3 initialV = generateRandom3DVector();
        BallObject* ball = new BallObject(glm::vec3(-30.0f + i * 1.0, 2.0f, -30.0f + i * 1.0), BALL_RADIUS, initialV, ResourceManager::GetTexture("marble"));
        ball->IsSphere = true;

        _balls.push_back(ball);
    }

    for (int i = 0; i < 60; i++) {
        glm::vec3 initialV = generateRandom3DVector();
        BallObject* ball = new BallObject(glm::vec3(-30.0f + i * 1.0, 10.0f, -30.0f + i * 1.0), BALL_RADIUS, initialV, ResourceManager::GetTexture("bmarble"));
        ball->IsSphere = true;

        _balls.push_back(ball);
    }

    for (int i = 0; i < 60; i++) {
        glm::vec3 initialV = generateRandom3DVector();
        BallObject* ball = new BallObject(glm::vec3(-30.0f + i * 1.0, 18.0f, -30.0f + i * 1.0), BALL_RADIUS, initialV, ResourceManager::GetTexture("gmarble"));
        ball->IsSphere = true;

        _balls.push_back(ball);
    }

    for (int i = 0; i < 60; i++) {
        glm::vec3 initialV = generateRandom3DVector();
        BallObject* ball = new BallObject(glm::vec3(-30.0f + i * 1.0, 4.0f, 30.0f - i * 1.0), BALL_RADIUS, initialV, ResourceManager::GetTexture("marble"));
        ball->IsSphere = true;

        _balls.push_back(ball);
    }

    for (int i = 0; i < 60; i++) {
        glm::vec3 initialV = generateRandom3DVector();
        BallObject* ball = new BallObject(glm::vec3(-30.0f + i * 1.0, 12.0f, 30.0f - i * 1.0), BALL_RADIUS, initialV, ResourceManager::GetTexture("bmarble"));
        ball->IsSphere = true;

        _balls.push_back(ball);
    }

    for (int i = 0; i < 60; i++) {
        glm::vec3 initialV = generateRandom3DVector();
        BallObject* ball = new BallObject(glm::vec3(-30.0f + i * 1.0, 20.0f, 30.0f - i * 1.0), BALL_RADIUS, initialV, ResourceManager::GetTexture("gmarble"));
        ball->IsSphere = true;

        _balls.push_back(ball);
    }

    for (int i = 0; i < 60; i++) {
        glm::vec3 initialV = generateRandom3DVector();
        BallObject* ball = new BallObject(glm::vec3(-30.0f + i * 1.0, 6.0f, 0.0f), BALL_RADIUS, initialV, ResourceManager::GetTexture("marble"));
        ball->IsSphere = true;

        _balls.push_back(ball);
    }

    for (int i = 0; i < 60; i++) {
        glm::vec3 initialV = generateRandom3DVector();
        BallObject* ball = new BallObject(glm::vec3(-30.0f + i * 1.0, 14.0f, 0.0f), BALL_RADIUS, initialV, ResourceManager::GetTexture("bmarble"));
        ball->IsSphere = true;

        _balls.push_back(ball);
    }

    for (int i = 0; i < 60; i++) {
        glm::vec3 initialV = generateRandom3DVector();
        BallObject* ball = new BallObject(glm::vec3(-30.0f + i * 1.0, 22.0f, 0.0f), BALL_RADIUS, initialV, ResourceManager::GetTexture("gmarble"));
        ball->IsSphere = true;

        _balls.push_back(ball);
    }

    for (int i = 0; i < 60; i++) {
        glm::vec3 initialV = generateRandom3DVector();
        BallObject* ball = new BallObject(glm::vec3(0.0f, 8.0f, -30.0f + i * 1.0), BALL_RADIUS, initialV, ResourceManager::GetTexture("marble"));
        ball->IsSphere = true;

        _balls.push_back(ball);
    }

    for (int i = 0; i < 60; i++) {
        glm::vec3 initialV = generateRandom3DVector();
        BallObject* ball = new BallObject(glm::vec3(0.0f, 16.0f, -30.0f + i * 1.0f), BALL_RADIUS, initialV, ResourceManager::GetTexture("bmarble"));
        ball->IsSphere = true;

        _balls.push_back(ball);
    }

}

void Game::Update(float dt)
{

    // Movement threads
    for (int i = 0; i < _balls.size(); i++) {
        _balls[i]->Move(dt);
    }

    DetectCollisions();

    //Node* root = Node::Create(0, 0, 90, 90, 1);
    //ConstructQuadTree(root, dt);

    // Free memory of the quadtree after usage
    //delete root;
}

void Game::ProcessInput(float dt)
{
    if (this->State == GAME_ACTIVE)
    {
        if (this->Keys[GLFW_KEY_W])
        {
            camera.ProcessKeyboard(FORWARD, dt);
        }
        if (this->Keys[GLFW_KEY_S])
        {
            camera.ProcessKeyboard(BACKWARD, dt);
        }
        if (this->Keys[GLFW_KEY_A])
        {
            camera.ProcessKeyboard(LEFT, dt);
        }
        if (this->Keys[GLFW_KEY_D])
        {
            camera.ProcessKeyboard(RIGHT, dt);
        }
        if (this->Keys[GLFW_KEY_SPACE])
        {
            for (int i = 0; i < _balls.size(); i++)
            {
                
                glm::vec3 randomV = generateRandom3DVector();
                _balls[i]->Velocity = randomV;
                _balls[i]->Velocity.y = 9.8f;
            }
        }
    }
}

void Game::Render()
{
    glm::mat4 view = camera.GetViewMatrix();
    ResourceManager::GetShader("sprite").Use().SetMatrix4("view", view);

    for (int i = 0; i < 1; i++)
    {
        Walls[i]->Draw(*Renderer);
    }

    for (unsigned int i = 0; i < _balls.size(); i++) {
        _balls[i]->Draw(*Renderer);
    }
}

void Game::ConstructQuadTree(Node* root, float dt)
{
    root->Set(0, 0, 90, 90, 1);

    for (BallObject* ball : _balls)
    {
        root->Add(ball);
    }

    // Detect and resolve collisions in the quadtree
    root->DetectCollisions();
}


void Game::MultithreadedUpdateMovement(float dt, int startIdx, int endIdx) {
    for (int i = startIdx; i < endIdx; i++) {
        _balls[i]->Move(dt);
    }
}