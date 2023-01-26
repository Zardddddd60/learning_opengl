#include "vertex_buffer.h"
#include <glad/glad.h>
#include <iostream>

VertexBuffer::VertexBuffer()
{
    glGenBuffers(1, &m_VertextBufferId);
}

VertexBuffer::VertexBuffer(const void* data, unsigned int size)
{
    glGenBuffers(1, &m_VertextBufferId);
    glBindBuffer(GL_ARRAY_BUFFER, m_VertextBufferId);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

void VertexBuffer::bindData(const void* data, unsigned int size) const
{
    glBindBuffer(GL_ARRAY_BUFFER, m_VertextBufferId);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

VertexBuffer::~VertexBuffer()
{
    // glDeleteBuffers(1, &m_VertextBufferId);
}

void VertexBuffer::bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, m_VertextBufferId);
}

void VertexBuffer::unbind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
