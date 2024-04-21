/** @file Object.hpp
 *  @brief Sets up an OpenGL camera..
 *
 *  More...
 *
 *  @author Mike
 *  @bug No known bugs.
 */
#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <glad/glad.h>

#include <vector>
#include <string>

#include "Shader.hpp"
#include "VertexBufferLayout.hpp"
#include "Texture.hpp"
#include "Transform.hpp"
#include "Geometry.hpp"
#include "Camera.hpp"

#include "glm/vec3.hpp"
#include "glm/gtc/matrix_transform.hpp"

// Purpose:
// An abstraction to create multiple objects
//
//
class Object
{
public:
    // Object Constructor
    Object();
    // Object destructor
    ~Object();
    // Create a 3D Plane
    void Make3DPlane(std::vector<std::string> tokens, unsigned int lod, float length);
    // Updates and transformations applied to object
    void Update(unsigned int screenWidth, unsigned int screenHeight, Camera *camera, unsigned int cubemapTexture, bool flipNormals, bool defaultShader = true);
    // How to draw the object
    void Render(unsigned int cubemapTexture, bool defaultShader = true);
    // Returns an objects transform
    Transform &GetTransform();

private:
    // Helper method for when we are ready to draw or update our object
    void Bind(bool defaultShader, unsigned int cubemapTexture);

    // Object vertices
    std::vector<GLfloat> m_vertices;
    // Object indices
    std::vector<GLuint> m_indices;

    // The first shader of the object
    Shader m_shader;
    // The second (possibly unused) shader of the objecct
    Shader m_shader2;
    // For now we have one buffer per object.
    VertexBufferLayout m_vertexBufferLayout;
    // Store the objects transformations
    Transform m_transform;
    // Store the 'camera' projection
    glm::mat4 m_projectionMatrix;
    // Store the objects Geometry
    Geometry m_geometry;
};

#endif
