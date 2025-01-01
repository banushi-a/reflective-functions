#ifndef SDLGRAPHICSPROGRAM_HPP
#define SDLGRAPHICSPROGRAM_HPP

// ==================== Libraries ==================
// Depending on the operating system we use
// The paths to SDL are actually different.
// The #define statement should be passed in
// when compiling using the -D argument.
// This gives an example of how a programmer
// may support multiple platforms with different
// dependencies.
#if defined(LINUX) || defined(MINGW)
#include <SDL2/SDL.h>
#else // This works for Mac
#include <SDL.h>
#endif

#include "Camera.hpp"
#include "Skybox.hpp"
#include "Lines.hpp"

#include <vector>
#include <string>

// The glad library helps setup OpenGL extensions.
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Purpose:
// This class sets up a full graphics program using SDL
//
//
//
class SDLGraphicsProgram
{
public:
    // Constructor
    SDLGraphicsProgram(int w, int h, std::vector<std::string> heightFunctions, unsigned int lod, float length);
    // Destructor
    ~SDLGraphicsProgram();
    // Setup OpenGL
    bool InitGL();
    // Per frame update
    void Update();
    // Renders shapes to the screen
    void Render();
    // loop that runs forever
    void Loop();
    // Get Pointer to Window
    SDL_Window *GetSDLWindow();
    // Helper Function to Query OpenGL information.
    void GetOpenGLVersionInfo();

private:
    // Screen dimension constants
    int m_screenWidth;
    int m_screenHeight;
    // The window we'll be rendering to
    SDL_Window *m_window;
    // Camera
    Camera m_Camera;
    // Skybox
    Skybox m_skybox;
    // Cubemap Texture
    unsigned int m_cubemapTexture;
    // Lines Object
    Lines m_lines;
    // Wireframe or Solid
    GLenum m_polygonMode;
    // Currently selected surfaces / curves
    bool m_showSurface[9] = {true, false, false, false, false, false, false, false, false};
    // Show the normals?
    bool m_showNormals = false;
    // Flip the normals?
    bool m_flipNormals = false;
    // OpenGL context
    SDL_GLContext m_openGLContext;
};

#endif
