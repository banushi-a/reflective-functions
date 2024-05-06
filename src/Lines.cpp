#include "Lines.hpp"

#include <stack>
#include <vector>
#include <set>

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

float evaluateReversePolishFunction2(std::vector<std::string> tokens)
{
    std::stack<float> stack;

    std::set<std::string> binaryOperators = {"*", "/", "+", "-", "pow"};
    std::set<std::string> unaryOperators = {"sin", "cos", "sqrt", "abs"};
    std::set<std::string> constants = {"e", "pi"};

    bool isImaginary = false;

    for (std::vector<std::string>::iterator t = tokens.begin(); t != tokens.end(); ++t)
    {
        if (binaryOperators.find(*t) != binaryOperators.end())
        {
            float b = stack.top();
            stack.pop();
            float a = stack.top();
            stack.pop();
            if (!t->compare("*"))
            {
                stack.push(a * b);
            }
            else if (!t->compare("/"))
            {
                stack.push(a / b);
            }
            else if (!t->compare("+"))
            {
                stack.push(a + b);
            }
            else if (!t->compare("-"))
            {
                stack.push(a - b);
            }
            else if (!t->compare("pow"))
            {
                stack.push(std::pow(a, b));
            }
        }
        else if (unaryOperators.find(*t) != unaryOperators.end())
        {
            float a = stack.top();
            stack.pop();
            if (!t->compare("sin"))
            {
                stack.push(std::sin(a));
            }
            else if (!t->compare("cos"))
            {
                stack.push(std::cos(a));
            }
            else if (!t->compare("sqrt"))
            {
                if (a < 0)
                {
                    isImaginary = true;
                }
                stack.push(std::sqrt(a));
            }
            else if (!t->compare("abs"))
            {
                stack.push(std::abs(a));
            }
        }
        else if (constants.find(*t) != constants.end())
        {
            if (!t->compare("e"))
            {
                stack.push(2.71828182845904523536f);
            }
            else if (!t->compare("pi"))
            {
                stack.push(3.14159265358979323846f);
            }
        }
        else
        {
            stack.push(std::stof(*t));
        }
    }

    if (isImaginary)
        return 0;

    return stack.top();
}

void Lines::MakeFunctionLines(std::vector<std::string> tokens, unsigned int lod, float length)
{
    float lowerBound = -1 * length;
    float upperBound = length;

    float interval = (upperBound - lowerBound) / lod;

    // Make the rows
    for (float r = lowerBound; r <= upperBound; r += 1.0f)
    {
        bool isPrev = false;
        float prevX, prevY, prevZ;
        // Make a unit line with segments based on level of detail
        for (float c = 0; c <= lod; c++)
        {
            float x = lowerBound + interval * c;
            float z = r;
            std::vector<std::string> newTokens;
            std::vector<std::string> newTokens2;
            for (const auto &token : tokens)
            {
                if (token == "x")
                {
                    newTokens.push_back(std::to_string(x));
                }
                else if (token == "z")
                {
                    newTokens.push_back(std::to_string(z));
                }
                else
                {
                    newTokens.push_back(token);
                }
            }
            float y = evaluateReversePolishFunction2(newTokens);
            if (isPrev)
            {
                MakeLine(prevX, prevY + 0.005f, prevZ, x, y + 0.005f, z, 0.4f, 0.0f, 0.0f);
            }
            prevX = x;
            prevY = y;
            prevZ = z;
            isPrev = true;
        }
    }

    // Make the columns
    for (float r = lowerBound; r <= upperBound; r += 1.0f)
    {
        bool isPrev = false;
        float prevX, prevY, prevZ;
        // Make a unit line with segments based on level of detail
        for (float c = 0; c <= lod; c++)
        {
            float x = r;
            float z = lowerBound + interval * c;
            std::vector<std::string> newTokens;
            std::vector<std::string> newTokens2;
            for (const auto &token : tokens)
            {
                if (token == "x")
                {
                    newTokens.push_back(std::to_string(x));
                }
                else if (token == "z")
                {
                    newTokens.push_back(std::to_string(z));
                }
                else
                {
                    newTokens.push_back(token);
                }
            }
            float y = evaluateReversePolishFunction2(newTokens);
            if (isPrev)
            {
                MakeLine(prevX, prevY + 0.005f, prevZ, x, y + 0.005f, z, 0.4f, 0.0f, 0.0f);
            }
            prevX = x;
            prevY = y;
            prevZ = z;
            isPrev = true;
        }
    }

    // for (int r = lowerBound; r <= upperBound; r++)
    // {
    //     for (int c = lowerBound; c <= upperBound; c++)
    //     {
    //         float x = lowerBound + interval * c;
    //         float z = lowerBound + interval * r;
    //         std::vector<std::string> newTokens1; // (x, z) => (x, z)
    //         std::vector<std::string> newTokens2; // (x, z) => (x - i, z)
    //         std::vector<std::string> newTokens3; // (x, z) => (x, z - i)
    //         std::vector<std::string> newTokens4; // (x, z) => (x - i, z - i)
    //         for (const auto &token : tokens)
    //         {
    //             if (token == "x")
    //             {
    //                 newTokens1.push_back(std::to_string(x));

    //                 newTokens2.push_back(std::to_string(x));
    //                 newTokens2.push_back(std::to_string(interval));
    //                 newTokens2.push_back("+");

    //                 newTokens3.push_back(std::to_string(x));

    //                 newTokens4.push_back(std::to_string(x));
    //                 newTokens4.push_back(std::to_string(interval));
    //                 newTokens4.push_back("+");
    //             }
    //             else if (token == "z")
    //             {
    //                 newTokens1.push_back(std::to_string(z));

    //                 newTokens2.push_back(std::to_string(z));

    //                 newTokens3.push_back(std::to_string(z));
    //                 newTokens3.push_back(std::to_string(interval));
    //                 newTokens3.push_back("+");

    //                 newTokens4.push_back(std::to_string(z));
    //                 newTokens4.push_back(std::to_string(interval));
    //                 newTokens4.push_back("+");
    //             }
    //             else
    //             {
    //                 newTokens1.push_back(token);
    //                 newTokens2.push_back(token);
    //                 newTokens3.push_back(token);
    //                 newTokens4.push_back(token);
    //             }
    //         }
    //         float y1 = evaluateReversePolishFunction2(newTokens1);
    //         float y2 = evaluateReversePolishFunction2(newTokens2);
    //         float y3 = evaluateReversePolishFunction2(newTokens3);
    //         float y4 = evaluateReversePolishFunction2(newTokens4);
    //         MakeLine(x, y1, z, x + interval, y2, z, 1.0f, 0.0f, 0.0f);
    //         MakeLine(x, y1, z, x, y3, z + interval, 0.0f, 1.0f, 0.0f);
    //         MakeLine(x + interval, y2, z, x + interval, y4, z + interval, 0.0f, 0.0f, 1.0f);
    //         MakeLine(x, y3, z + interval, x + interval, y4, z + interval, 1.0f, 1.0f, 1.0f);
    //     }
    // }
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
