#include <iostream>
#include "texture.h"
#include "vendors/stb_image/stb_image.h"
#include <glad/glad.h>

Texture::Texture(const std::string& filepath)
{
    m_Filepath = filepath;

    stbi_set_flip_vertically_on_load(true);
    glGenTextures(1, &m_TextureId);
    glBindTexture(GL_TEXTURE_2D, m_TextureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    unsigned char* data = stbi_load(filepath.c_str(), &m_Width, &m_Height, &m_BPP, 0);
    if (data)
    {
        GLenum format = GL_RGB;
        if (m_BPP == 1)
        {
            format = GL_RED;
        }
        else if (m_BPP == 3)
        {
            format = GL_RGB;
        }
        else if (m_BPP == 4)
        {
            format = GL_RGBA;
        }
        glTexImage2D(GL_TEXTURE_2D, 0, format, m_Width, m_Height, 0, format, GL_UNSIGNED_BYTE, data);
        // glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
}

Texture::~Texture()
{
    glDeleteTextures(1, &m_TextureId);
}

void Texture::bind(unsigned int slot) const
{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, m_TextureId);
}

void Texture::unbind() const
{
    glBindTexture(GL_TEXTURE_2D, 0);
}
