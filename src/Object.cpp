#include "Object.hpp"
#include "Error.hpp"
#include "Camera.hpp"

#include <cmath>
#include <stack>
#include <vector>
#include <set>
#include <iostream>

Object::Object()
{
}

Object::~Object()
{
}

// Leetcode in real life :) [https://leetcode.com/problems/evaluate-reverse-polish-notation/description/]
float evaluateReversePolishFunction(std::vector<std::string> tokens)
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

void Object::Make3DPlane(std::vector<std::string> tokens, unsigned int lod, float length)
{

        /*
        example of equation vs. tokens for a hyperbolic parabola:

        equation: "y = x^2 - z^2"
        tokens: ["x", "x", "*", "z", "z", "*", "-"]

        */

        float lowerBound = -1 * length;
        float upperBound = length;

        float interval = (upperBound - lowerBound) / lod;

        for (int r = 0; r < (lod + 1); r++)
        {
                for (int c = 0; c < (lod + 1); c++)
                {
                        float x = lowerBound + interval * c;
                        float z = lowerBound + interval * r;
                        std::vector<std::string> newTokens;
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
                        float y = evaluateReversePolishFunction(newTokens);
                        m_geometry.AddVertex(x, y, z, 1.0f / lod * c, 1.0f / lod * r);
                }
        }

        for (int r = 0; r < lod; r++)
        {
                for (int c = 0; c < lod; c++)
                {
                        m_geometry.MakeTriangle((r * (lod + 1)) + c, (r * (lod + 1)) + c + 1, ((r + 1) * (lod + 1)) + c);
                        m_geometry.MakeTriangle((r * (lod + 1)) + c + 1, ((r + 1) * (lod + 1)) + c + 1, ((r + 1) * (lod + 1)) + c);
                }
        }

        // This is a helper function to generate all of the geometry
        m_geometry.Gen();

        // This generates the lines overlaying the geometry
        m_lines.MakeFunctionLines(tokens, lod, length);
        m_lines.Gen();

        // Create a buffer and set the stride of information
        // NOTE: How we are leveraging our data structure in order to very cleanly
        //       get information into and out of our data structure.
        m_vertexBufferLayout.CreateNormalBufferLayout(m_geometry.GetBufferDataSize(),
                                                      m_geometry.GetIndicesSize(),
                                                      m_geometry.GetBufferDataPtr(),
                                                      m_geometry.GetIndicesDataPtr());

        // Setup shaders
        std::string vertexShader = m_shader.LoadShader("./shaders/vert.glsl");
        std::string fragmentShader = m_shader.LoadShader("./shaders/frag.glsl");

        std::string normalVertexShader = m_shader2.LoadShader("./shaders/normalVert.glsl");
        std::string normalGeometryShader = m_shader2.LoadShader("./shaders/normalGeom.glsl");
        std::string normalFragmentShader = m_shader2.LoadShader("./shaders/normalFrag.glsl");

        // Actually create our shaders
        m_shader.CreateShader(vertexShader, fragmentShader);
        m_shader2.CreateShader(normalVertexShader, normalGeometryShader, normalFragmentShader);
}

// Bind everything we need in our object
// Generally this is called in update() and render()
// before we do any actual work with our object
void Object::Bind(bool defaultShader, unsigned int cubemapTexture)
{
        // Select our appropriate shader
        if (defaultShader)
        {
                m_shader.Bind();
        }
        else
        {
                m_shader2.Bind();
        }
        // Make sure we are updating the correct 'buffers'
        m_vertexBufferLayout.Bind();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
}

void Object::Update(unsigned int screenWidth, unsigned int screenHeight, Camera *camera, unsigned int cubemapTexture, bool flipNormals, bool defaultShader)
{
        // Call our helper function to just bind everything
        Bind(defaultShader, cubemapTexture);

        // Note that we set the value to 0, because we have bound
        // Here we apply the 'view' matrix which creates perspective.
        // The first argument is 'field of view'
        // Then perspective
        // Then the near and far clipping plane.
        m_projectionMatrix = glm::perspective(glm::radians(45.0f), ((float)screenWidth) / ((float)screenHeight), 0.1f, 50.0f);

        glm::mat4 viewMatrix = camera->GetViewMatrix();

        // Set the uniforms in our current shader
        if (defaultShader)
        {
                m_shader.SetUniformMatrix4fv("modelTransformMatrix", m_transform.GetTransformMatrix());
                m_shader.SetUniformMatrix4fv("viewMatrix", &viewMatrix[0][0]);
                m_shader.SetUniformMatrix4fv("projectionMatrix", &m_projectionMatrix[0][0]);
                // Create a first 'light'
                // Set in a light source position
                m_shader.SetUniform3f("lightPos", 0.0f, 1.0f, 1.0f);
                // Set a view and a vector
                m_shader.SetUniform3f("viewPos", 0.0f, 0.0f, 0.0f);
                if (flipNormals)
                {
                        m_shader.SetUniform1i("flipNormals", 1);
                }
                else
                {
                        m_shader.SetUniform1i("flipNormals", 0);
                }

                m_lines.GetTransform().LoadIdentity();
                m_lines.GetTransform().ApplyTransform(m_transform);
                m_lines.Update(screenWidth, screenHeight, camera);
        }
        else
        {
                m_shader2.SetUniformMatrix4fv("modelTransformMatrix", m_transform.GetTransformMatrix());
                m_shader2.SetUniformMatrix4fv("viewMatrix", &viewMatrix[0][0]);
                m_shader2.SetUniformMatrix4fv("projectionMatrix", &m_projectionMatrix[0][0]);
                // Create a first 'light'
                // Set in a light source position
                m_shader2.SetUniform3f("lightPos", 0.0f, 1.0f, 1.0f);
                // Set a view and a vector
                m_shader2.SetUniform3f("viewPos", 0.0f, 0.0f, 0.0f);
                if (flipNormals)
                {
                        m_shader2.SetUniform1i("flipNormals", 1);
                }
                else
                {
                        m_shader2.SetUniform1i("flipNormals", 0);
                }
        }
}

// Render our geometry
void Object::Render(unsigned int cubemapTexture, bool defaultShader)
{
        // Call our helper function to just bind everything
        Bind(defaultShader, cubemapTexture);
        // Render data
        glDrawElements(GL_TRIANGLES,
                       m_geometry.GetIndicesSize(), // The number of indices, not triangles.
                       GL_UNSIGNED_INT,             // Make sure the data type matches
                       nullptr);                    // Offset pointer to the data.
                                                    // nullptr because we are currently bound

        m_lines.Render();
}

// Returns the actual transform stored in our object
// which can then be modified
Transform &Object::GetTransform()
{
        return m_transform;
}
