#pragma once

#include <vector>
#include <glad/glad.h>
#include "glm/glm.hpp"

struct VertexBufferElement
{
    unsigned int type;
    unsigned int count;
    unsigned char normalized;
    bool isInstancing = false;

    static unsigned int getSizeOfByte(unsigned int type)
    {
        switch (type)
        {
            case GL_FLOAT:
            case GL_UNSIGNED_INT:
                return 4;
            case GL_UNSIGNED_BYTE:
                return 1;
        }
        // ASSERT(false);
        return 0;
    }
};


class VertexBufferLayout
{
private:
    std::vector<VertexBufferElement> m_Elements;
    unsigned int m_Stride;
public:
    VertexBufferLayout()
        : m_Stride(0) {};

    template<typename T>
    void push(unsigned int count, bool isInstancing = false)
    {
        // static_assert(false);
    }

    inline const std::vector<VertexBufferElement> getElements() const&
    {
        return m_Elements;
    }

    inline unsigned int getStride() const 
    {
        return m_Stride;
    }
};

template<>
inline void VertexBufferLayout::push<float>(unsigned int count, bool isInstancing)
{
    VertexBufferElement vbe = {
        GL_FLOAT, count, GL_FALSE
    };
    m_Elements.push_back(vbe);
    m_Stride += count * VertexBufferElement::getSizeOfByte(GL_FLOAT);
}

template<>
inline void VertexBufferLayout::push<unsigned int>(unsigned int count, bool isInstancing)
{
    VertexBufferElement vbe = {
        GL_UNSIGNED_INT, count, GL_FALSE
    };
    m_Elements.push_back(vbe);
    m_Stride += count * VertexBufferElement::getSizeOfByte(GL_UNSIGNED_INT);

}

template<>
inline void VertexBufferLayout::push<unsigned char>(unsigned int count, bool isInstancing)
{
    VertexBufferElement vbe = {
        GL_UNSIGNED_BYTE, count, GL_TRUE
    };
    m_Elements.push_back(vbe);
    m_Stride += count * VertexBufferElement::getSizeOfByte(GL_UNSIGNED_BYTE);
}

template<>
inline void VertexBufferLayout::push<glm::mat4>(unsigned int count, bool isInstancing)
{
    // 要看4个gl_float是不是和sizeof(glm::vec4相同)，否则offset会计算不对
    VertexBufferElement vbe = {
        GL_FLOAT, 4, GL_FALSE, isInstancing
    };
    m_Elements.push_back(vbe);
    m_Elements.push_back(vbe);
    m_Elements.push_back(vbe);
    m_Elements.push_back(vbe);
    m_Stride += 16 * VertexBufferElement::getSizeOfByte(GL_FLOAT);
}

template<>
inline void VertexBufferLayout::push<glm::vec2>(unsigned int count, bool isInstancing)
{
    // 要看4个gl_float是不是和sizeof(glm::vec4相同)，否则offset会计算不对
    VertexBufferElement vbe = {
        GL_FLOAT, 2, GL_FALSE, isInstancing
    };
    m_Elements.push_back(vbe);
    m_Stride += count * VertexBufferElement::getSizeOfByte(GL_FLOAT);
}
