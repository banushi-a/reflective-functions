#include "Skybox.hpp"

#include <vector>
#include <string>
#include <iostream>
#include "Image.hpp"

Skybox::Skybox()
{
}

Skybox::~Skybox()
{
}

/*
From [https://learnopengl.com/Advanced-OpenGL/Cubemaps]
*/
unsigned int Skybox::loadCubemap(std::vector<std::string> faces)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height;

    for (unsigned int i = 0; i < faces.size(); i++)
    {
        Image currImage = Image(faces[i]);
        currImage.LoadPPM(false);
        height = currImage.GetHeight();
        width = currImage.GetWidth();
        uint8_t *data = currImage.GetPixelDataPtr();
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                         0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        }
        else
        {
            std::cout << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
        }
    }
    // Make sure no bytes are padded:
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    // Select GL_MODULATE to mix texture with polygon color for shading:
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    // Unbind the texture cube map.
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    return textureID;
}

void Skybox::connectShaders()
{

    float skyboxVertices[] = {
        // positions
        -100.0f,
        100.0f,
        -100.0f,
        -100.0f,
        -100.0f,
        -100.0f,
        100.0f,
        -100.0f,
        -100.0f,
        100.0f,
        -100.0f,
        -100.0f,
        100.0f,
        100.0f,
        -100.0f,
        -100.0f,
        100.0f,
        -100.0f,

        -100.0f,
        -100.0f,
        100.0f,
        -100.0f,
        -100.0f,
        -100.0f,
        -100.0f,
        100.0f,
        -100.0f,
        -100.0f,
        100.0f,
        -100.0f,
        -100.0f,
        100.0f,
        100.0f,
        -100.0f,
        -100.0f,
        100.0f,

        100.0f,
        -100.0f,
        -100.0f,
        100.0f,
        -100.0f,
        100.0f,
        100.0f,
        100.0f,
        100.0f,
        100.0f,
        100.0f,
        100.0f,
        100.0f,
        100.0f,
        -100.0f,
        100.0f,
        -100.0f,
        -100.0f,

        -100.0f,
        -100.0f,
        100.0f,
        -100.0f,
        100.0f,
        100.0f,
        100.0f,
        100.0f,
        100.0f,
        100.0f,
        100.0f,
        100.0f,
        100.0f,
        -100.0f,
        100.0f,
        -100.0f,
        -100.0f,
        100.0f,

        -100.0f,
        100.0f,
        -100.0f,
        100.0f,
        100.0f,
        -100.0f,
        100.0f,
        100.0f,
        100.0f,
        100.0f,
        100.0f,
        100.0f,
        -100.0f,
        100.0f,
        100.0f,
        -100.0f,
        100.0f,
        -100.0f,

        -100.0f,
        -100.0f,
        -100.0f,
        -100.0f,
        -100.0f,
        100.0f,
        100.0f,
        -100.0f,
        -100.0f,
        100.0f,
        -100.0f,
        -100.0f,
        -100.0f,
        -100.0f,
        100.0f,
        100.0f,
        -100.0f,
        100.0f,
    };

    // Setup shaders
    std::string vertexShader = m_shader.LoadShader("./shaders/skyboxVert.glsl");
    std::string fragmentShader = m_shader.LoadShader("./shaders/skyboxFrag.glsl");
    m_shader.CreateShader(vertexShader, fragmentShader);
    m_shader.Bind();

    // skybox VAO
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
}

void Skybox::render()
{
    glDepthFunc(GL_LEQUAL); // change depth function so depth test passes when values are equal to depth buffer's content
    m_shader.Bind();
    glm::mat4 view = glm::mat4(glm::mat3(m_camera->GetViewMatrix())); // remove translation from the view matrix
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), ((float)m_width) / ((float)m_height), 0.1f, 180.0f);
    glUniformMatrix4fv(glGetUniformLocation(m_shader.GetID(), "view"), 1, GL_FALSE, &view[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(m_shader.GetID(), "projection"), 1, GL_FALSE, &projection[0][0]);
    //  skybox cube
    glBindVertexArray(m_VAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubemapTexture);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glDepthFunc(GL_LESS); // set depth function back to default
}