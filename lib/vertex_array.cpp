#include <iostream>
#include "vertex_array.h"
#include "vertex_buffer_layout.h"
#include <glad/glad.h>

VertexArray::VertexArray()
{
    glGenVertexArrays(1, &m_VertexArrayId);
}

VertexArray::~VertexArray()
{
    // glDeleteVertexArrays(1, &m_VertexArrayId);
}

void VertexArray::addBuffer(const VertexBuffer& vb, const VertexBufferLayout& vbl) const
{
    bind();
    vb.bind();

    const auto& elements = vbl.getElements();
    unsigned int offset = 0;

    for (unsigned int i = 0; i < elements.size(); i ++)
    {
        const auto& element = elements[i];
        glVertexAttribPointer(i, element.count, element.type, element.normalized, vbl.getStride(), (void*)offset);
        glEnableVertexAttribArray(i);
        offset += element.count * VertexBufferElement::getSizeOfByte(element.type);
    }
}

void VertexArray::bind() const
{
    glBindVertexArray(m_VertexArrayId);
}

void VertexArray::unbind() const
{
    glBindVertexArray(0);
}
