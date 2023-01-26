#include "index_buffer.h"
#include <glad/glad.h>
IndexBuffer::IndexBuffer()
{
    glGenBuffers(1, &m_IndexBufferId);
}

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int size)
{
    // m_Count = count;
    glGenBuffers(1, &m_IndexBufferId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBufferId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
};

IndexBuffer::~IndexBuffer()
{
    // glDeleteBuffers(1, &m_IndexBufferId);
};

void IndexBuffer::bind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBufferId);
};

void IndexBuffer::unbind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
};

void IndexBuffer::bindData(const unsigned int* data, unsigned int size) const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBufferId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}
