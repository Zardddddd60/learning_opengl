#pragma once

#include <string>

class Texture
{
private:
    unsigned int m_TextureId;
    std::string m_Filepath;
    int m_Width, m_Height, m_BPP;
public:
    Texture(const std::string& filepath);
    ~Texture();
    void bind(unsigned int slot = 0) const;
    void unbind() const;
};
