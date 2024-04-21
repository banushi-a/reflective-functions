#ifndef SKYBOX_HPP
#define SKYBOX_HPP

#include "Shader.hpp"
#include <vector>
#include <string>
#include "Camera.hpp"

#include "glm/gtc/matrix_transform.hpp"

// Purpose
// An abstraction to create a skybox for our scene

class Skybox
{
public:
    // Constructor
    Skybox();
    // Destructor
    ~Skybox();
    // Helper to load a cubemap
    unsigned int loadCubemap(std::vector<std::string> faces);
    // Initialize the skybox
    void connectShaders();
    // Renders the skybox
    void render();
    // Store the camera for view projection
    Camera *m_camera;
    int m_width;
    int m_height;
    unsigned int m_cubemapTexture;

private:
    // The shader for the skybox
    Shader m_shader;
    // Vertex Array Object ID
    GLuint m_VAO;
    // Vertex Buffer Object ID
    GLuint m_VBO;
};

#endif