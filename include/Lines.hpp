#ifndef LINES_HPP
#define LINES_HPP

#include "Camera.hpp"
#include "Shader.hpp"
#include "Transform.hpp"

#include <vector>
#include <glad/glad.h>

#include "glm/vec3.hpp"
#include "glm/gtc/matrix_transform.hpp"

class Lines
{
public:
    Lines();
    ~Lines();
    // Add a line from v_0 to v_1
    void MakeLine(float x0, float y0, float z0, float x1, float y1, float z1, float r, float g, float b);
    // Make a square grid of lines at y = 0
    void MakeGrid(float lowerBound, float upperBound);
    // Compile all the lines and send them to a shader
    void Gen();
    // Updates and transformations applied to object
    void Update(unsigned int screenWidth, unsigned int screenHeight, Camera *camera);
    // How to draw the object
    void Render();
    // Returns an objects transform
    Transform &GetTransform();

private:
    // Helper method for when we are ready to draw or update our object
    void Bind();

    // Object vertex positions and colors
    std::vector<GLfloat> m_bufferData;

    // The first shader of the object
    Shader m_shader;

    GLuint m_VAO;
    GLuint m_VBO;

    // Store the objects transformations
    Transform m_transform;
    // Store the 'camera' projection
    glm::mat4 m_projectionMatrix;
};

#endif