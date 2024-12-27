#ifndef SPRITERENDERER_H
#define SPRITERENDERER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

#include "Texture.h"
#include "Shader.h"


class SpriteRenderer
{
public:
    // Constructor (inits shaders/shapes)
    
    SpriteRenderer(const Shader& shader);
    // Destructor
    ~SpriteRenderer();
    
    // Renders a defined quad textured with given sprite
    void DrawSprite(const Texture2D& texture, glm::vec3 position, glm::vec3 size = glm::vec3(10.0f, 10.0f, 10.0f), glm::vec3 rotate = glm::vec3(0.0f), glm::vec3 color = glm::vec3(1.0f), bool isSphere = false);
private:
    // Render state
    Shader       shader;
    unsigned int quadVAO;
    unsigned int sphereVAO = 0;
    unsigned int indexCount;

    std::vector<unsigned int> sphereIndices;

    // Initializes and configures the quad's buffer and vertex attributes
    void initRenderData();
    void initSphereRenderData();
};

#endif