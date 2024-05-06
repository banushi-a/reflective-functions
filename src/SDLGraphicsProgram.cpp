#include "SDLGraphicsProgram.hpp"
#include "ObjectManager.hpp"
#include "Skybox.hpp"

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>

// Initialization function
// Returns a true or false value based on successful completion of setup.
// Takes in dimensions of window.
SDLGraphicsProgram::SDLGraphicsProgram(int w, int h, std::vector<std::string> heightFunctions, unsigned int lod, float length) : m_screenWidth(w), m_screenHeight(h)
{
    // Initialization flag
    bool success = true;
    // String to hold any errors that occur.
    std::stringstream errorStream;
    // The window we'll be rendering to
    m_window = NULL;

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        errorStream << "SDL could not initialize! SDL Error: " << SDL_GetError() << "\n";
        success = false;
    }
    else
    {
        // Use OpenGL 3.3 core
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        // We want to request a double buffer for smooth updating.
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

        // Create window
        m_window = SDL_CreateWindow("3D Function Visualizer",
                                    SDL_WINDOWPOS_UNDEFINED,
                                    SDL_WINDOWPOS_UNDEFINED,
                                    m_screenWidth,
                                    m_screenHeight,
                                    SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

        // Check if Window did not create.
        if (m_window == NULL)
        {
            errorStream << "Window could not be created! SDL Error: " << SDL_GetError() << "\n";
            success = false;
        }

        // Create an OpenGL Graphics Context
        m_openGLContext = SDL_GL_CreateContext(m_window);
        if (m_openGLContext == NULL)
        {
            errorStream << "OpenGL context could not be created! SDL Error: " << SDL_GetError() << "\n";
            success = false;
        }

        // Initialize GLAD Library
        if (!gladLoadGLLoader(SDL_GL_GetProcAddress))
        {
            errorStream << "Failed to iniitalize GLAD\n";
            success = false;
        }

        // Initialize OpenGL
        if (!InitGL())
        {
            errorStream << "Unable to initialize OpenGL!\n";
            success = false;
        }
    }

    // If initialization did not work, then print out a list of errors in the constructor.
    if (!success)
    {
        errorStream << "SDLGraphicsProgram::SDLGraphicsProgram - Failed to initialize!\n";
        std::string errors = errorStream.str();
        SDL_Log("%s\n", errors.c_str());
    }
    else
    {
        SDL_Log("SDLGraphicsProgram::SDLGraphicsProgram - No SDL, GLAD, or OpenGL, errors detected during initialization\n\n");
    }

    // SDL_LogSetAllPriority(SDL_LOG_PRIORITY_WARN); // Uncomment to enable extra debug support!
    GetOpenGLVersionInfo();

    SDL_WarpMouseInWindow(m_window, m_screenWidth / 2, m_screenHeight / 2);
    SDL_SetRelativeMouseMode(SDL_TRUE);

    // Setup our graphs
    for (int i = 0; i < heightFunctions.size(); ++i)
    {
        Object *curve = new Object;
        std::vector<std::string> tokens;
        // Parse user input
        std::istringstream iss(heightFunctions.at(i));
        std::string token;
        while (std::getline(iss, token, ' '))
        {
            tokens.push_back(token);
        }
        curve->Make3DPlane(tokens, lod, length);
        ObjectManager::Instance().AddObject(curve);
    }

    // Setup Camera
    m_Camera = Camera();

    // Setup our skybox
    m_skybox = Skybox();
    std::vector<std::string> faces =
        {
            "right.ppm",
            "left.ppm",
            "top.ppm",
            "bottom.ppm",
            "front.ppm",
            "back.ppm",
        };
    m_cubemapTexture = m_skybox.loadCubemap(faces);
    m_skybox.m_cubemapTexture = m_cubemapTexture;
    ObjectManager::Instance().m_cubemapTexture = m_cubemapTexture;
    m_skybox.m_camera = &m_Camera;
    m_skybox.m_width = w;
    m_skybox.m_height = h;
    m_skybox.connectShaders();

    // Setup our Axes
    m_lines.MakeLine(-50.0f, 0.0f, 0.0f, 50.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
    m_lines.MakeLine(0.0f, -50.0f, 0.0f, 0.0f, 50.0f, 0.0f, 0.0f, 1.0f, 0.0f);
    m_lines.MakeLine(0.0f, 0.0f, -50.0f, 0.0f, 0.0f, 50.0f, 0.0f, 0.0f, 1.0f);
    // m_lines.MakeGrid(-50.0f, 50.0f);
    m_lines.Gen();
}

// Proper shutdown of SDL and destroy initialized objects
SDLGraphicsProgram::~SDLGraphicsProgram()
{
    // Reclaim all of our objects
    ObjectManager::Instance().RemoveAll();

    // Destroy window
    SDL_DestroyWindow(m_window);
    // Point m_window to NULL to ensure it points to nothing.
    m_window = nullptr;
    // Quit SDL subsystems
    SDL_Quit();
}

// Initialize OpenGL
// Setup any of our shaders here.
bool SDLGraphicsProgram::InitGL()
{
    // Success flag
    bool success = true;

    return success;
}

// Update OpenGL
void SDLGraphicsProgram::Update()
{

    for (int i = 0; i < ObjectManager::Instance().GetNumObjects(); i++)
    {

        ObjectManager::Instance().GetObject(i).GetTransform().LoadIdentity();
        // Push back our surfaces a bit
        ObjectManager::Instance().GetObject(i).GetTransform().Translate(0.0f, 0.0f, -8.0f);
    }

    // Update all of the objects
    ObjectManager::Instance().UpdateAll(m_screenWidth, m_screenHeight, &m_Camera, m_flipNormals);

    // Update our axes
    m_lines.GetTransform().LoadIdentity();
    m_lines.GetTransform().Translate(0.0f, 0.0f, -8.0f);
    m_lines.Update(m_screenWidth, m_screenHeight, &m_Camera);
}

// Render
// The render function gets called once per loop
void SDLGraphicsProgram::Render()
{
    // Setup our OpenGL State machine
    // What we are doing, is telling opengl to create a depth(or Z-buffer)
    // for us that is stored every frame.
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);

    // Initialize clear color
    // This is the background of the screen.
    glViewport(0, 0, m_screenWidth, m_screenHeight);
    glClearColor(0.2f, 0.2f, 0.2f, 1.f);
    // Clear color buffer and Depth Buffer
    // Remember that the 'depth buffer' is our
    // z-buffer that figures out how far away items are every frame
    // and we have to do this every frame!
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    // Render all of our objects in a simple loop
    ObjectManager::Instance().RenderAt(m_showSurface, m_showNormals);

    // Render our axes
    m_lines.Render();

    // Render our skybox
    m_skybox.render();

    // Delay to slow things down just a bit!
    SDL_Delay(30); // FIXME
}

// Loops forever!
void SDLGraphicsProgram::Loop()
{
    // Main loop flag
    // If this is quit = 'true' then the program terminates.
    bool quit = false;
    // Event handler that handles various events in SDL
    // that are related to input and output
    SDL_Event e;
    // Enable text input
    SDL_StartTextInput();

    // Set the camera speed for how fast we move.
    float cameraSpeed = 0.5f;

    // While application is running
    while (!quit)
    {

        // Handle events on queue
        while (SDL_PollEvent(&e) != 0)
        {
            // User posts an event to quit
            // An example is hitting the "x" in the corner of the window.
            if (e.type == SDL_QUIT)
            {
                quit = true;
            }
            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)
            {
                std::cout << "ESC: Goodbye! (Leaving MainApplicationLoop())" << std::endl;
                quit = true;
            }
            // Handle keyboard input for the camera class
            if (e.type == SDL_MOUSEMOTION)
            {
                // Handle mouse movements
                int mouseX = e.motion.x;
                int mouseY = e.motion.y;
                m_Camera.MouseLook(mouseX, mouseY);
            }
            switch (e.type)
            {
            // Handle keyboard presses
            case SDL_KEYDOWN:
                switch (e.key.keysym.sym)
                {
                case SDLK_LEFT:
                    m_Camera.MoveLeft(cameraSpeed);
                    break;
                case SDLK_RIGHT:
                    m_Camera.MoveRight(cameraSpeed);
                    break;
                case SDLK_UP:
                    m_Camera.MoveForward(cameraSpeed);
                    break;
                case SDLK_DOWN:
                    m_Camera.MoveBackward(cameraSpeed);
                    break;
                case SDLK_LSHIFT:
                    m_Camera.MoveUp(cameraSpeed);
                    break;
                case SDLK_LCTRL:
                    m_Camera.MoveDown(cameraSpeed);
                    break;
                case SDLK_w:
                    if (m_polygonMode == GL_LINE)
                    {
                        std::cout << "Changing to GL_FILL" << std::endl;
                        m_polygonMode = GL_FILL;
                        m_showNormals = false;
                        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                    }
                    else
                    {
                        std::cout << "Changing to GL_LINE" << std::endl;
                        m_polygonMode = GL_LINE;
                        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                    }
                    break;
                case SDLK_q:
                    m_showNormals = !m_showNormals;
                    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                    break;
                case SDLK_e:
                    m_flipNormals = !m_flipNormals;
                    break;
                case SDLK_1:
                    m_showSurface[0] = !m_showSurface[0];
                    break;
                case SDLK_2:
                    m_showSurface[1] = !m_showSurface[1];
                    break;
                case SDLK_3:
                    m_showSurface[2] = !m_showSurface[2];
                    break;
                case SDLK_4:
                    m_showSurface[3] = !m_showSurface[3];
                    break;
                case SDLK_5:
                    m_showSurface[4] = !m_showSurface[4];
                    break;
                case SDLK_6:
                    m_showSurface[5] = !m_showSurface[5];
                    break;
                case SDLK_7:
                    m_showSurface[6] = !m_showSurface[6];
                    break;
                case SDLK_8:
                    m_showSurface[7] = !m_showSurface[7];
                    break;
                case SDLK_9:
                    m_showSurface[8] = !m_showSurface[8];
                    break;
                }
                break;
            }
        } // End SDL_PollEvent loop.

        // Update our scene
        Update();
        // Render using OpenGL
        Render();
        // Update screen of our specified window
        SDL_GL_SwapWindow(GetSDLWindow());
    }
    // Disable text input
    SDL_StopTextInput();
}

// Get Pointer to Window
SDL_Window *SDLGraphicsProgram::GetSDLWindow()
{
    return m_window;
}

// Helper Function to get OpenGL Version Information
void SDLGraphicsProgram::GetOpenGLVersionInfo()
{
    SDL_Log("========== CONTROLS ==========");
    SDL_Log("ARROW KEYS to Move Around");
    SDL_Log("Left Control and Left Shift to Move Up and Down");
    SDL_Log("W To Toggle Wireframe Mode");
    SDL_Log("Q To Toggles Normals");
    SDL_Log("E TO Reflect Normals");
    SDL_Log("1 - 9 To Toggle Loaded Surfaces");
    SDL_Log("ESC To Quit");
}
