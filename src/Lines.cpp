#include "Lines.hpp"

Lines::Lines()
{
}

Lines::~Lines()
{
}

void Lines::MakeLine(float x0, float y0, float z0, float x1, float y1, float z1, float r, float g, float b)
{
    m_bufferData.push_back(x0);
    m_bufferData.push_back(y0);
    m_bufferData.push_back(z0);
    m_bufferData.push_back(r);
    m_bufferData.push_back(g);
    m_bufferData.push_back(b);
    m_bufferData.push_back(x1);
    m_bufferData.push_back(y1);
    m_bufferData.push_back(z1);
    m_bufferData.push_back(r);
    m_bufferData.push_back(g);
    m_bufferData.push_back(b);
}

void Lines::MakeGrid(float lowerBound, float upperBound)
{
    unsigned int lod = 100;
    float interval = (upperBound - lowerBound) / lod;

    for (int r = 0; r < (lod + 1); r++)
    {
        for (int c = 0; c < (lod + 1); c++)
        {
            float x = lowerBound + interval * c;
            float z = lowerBound + interval * r;
            MakeLine(x, 0.0f, z, x + interval, 0.0f, z, 0.6f, 0.6f, 0.6f);
            MakeLine(x, 0.0f, z, x, 0.0f, z + interval, 0.6f, 0.6f, 0.6f);
            MakeLine(x + interval, 0.0f, z, x + interval, 0.0f, z + interval, 0.6f, 0.6f, 0.6f);
            MakeLine(x, 0.0f, z + interval, x + interval, 0.0f, z + interval, 0.6f, 0.6f, 0.6f);
        }
    }
}

void Lines::Gen()
{
    // Vertex Arrays (VAO)
    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    // Vertex Buffer Object (VBO)
    // Create and bind a buffer
    glGenBuffers(1, &m_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, m_bufferData.size() * sizeof(float), m_bufferData.data(), GL_STATIC_DRAW);

    // Setup shaders
    std::string vertexShader = m_shader.LoadShader("./shaders/lineVert.glsl");
    std::string fragmentShader = m_shader.LoadShader("./shaders/lineFrag.glsl");

    // Actually create our shaders
    m_shader.CreateShader(vertexShader, fragmentShader);
}

// Bind everything we need in our object
// Generally this is called in update() and render()
// before we do any actual work with our object
void Lines::Bind()
{
    m_shader.Bind();
    // Make sure we are updating the correct 'buffers'
    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

    // Get an index for the position attribute
    GLuint loc = glGetAttribLocation(m_shader.GetID(), "position");
    glEnableVertexAttribArray(loc);
    // Associate the attribute with the data in the buffer
    // This implicitly refers to the currently bound GL_ARRAY_BUFFER
    glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, 0);

    // Get an index for the color attribute
    loc = glGetAttribLocation(m_shader.GetID(), "color");
    glEnableVertexAttribArray(loc);
    // Associate the attribute with the data in the buffer
    // This implicitly refers to the currently bound GL_ARRAY_BUFFER
    glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (char *)(sizeof(float) * 3));
}

void Lines::Update(unsigned int screenWidth, unsigned int screenHeight, Camera *camera)
{
    Bind();

    m_projectionMatrix = glm::perspective(glm::radians(45.0f), ((float)screenWidth) / ((float)screenHeight), 0.1f, 50.0f);
    glm::mat4 viewMatrix = camera->GetViewMatrix();

    m_shader.SetUniformMatrix4fv("modelTransformMatrix", m_transform.GetTransformMatrix());
    m_shader.SetUniformMatrix4fv("viewMatrix", &viewMatrix[0][0]);
    m_shader.SetUniformMatrix4fv("projectionMatrix", &m_projectionMatrix[0][0]);

    glLineWidth(2.0f);
}

void Lines::Render()
{
    Bind();

    glDrawArrays(GL_LINES, 0, m_bufferData.size());
}

// Returns the actual transform stored in our object
// which can then be modified
Transform &Lines::GetTransform()
{
    return m_transform;
}
