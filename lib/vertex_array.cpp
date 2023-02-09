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

void VertexArray::addBuffer(const VertexBuffer& vb, const VertexBufferLayout& vbl)
{
    bind();
    vb.bind();

    const auto& elements = vbl.getElements();
    unsigned int offset = 0;

    for (unsigned int i = 0; i < elements.size(); i ++)
    {
        const unsigned int currentAttribIndex = m_EnabledAttrib + i;
        const auto& element = elements[i];
        glEnableVertexAttribArray(currentAttribIndex);
        glVertexAttribPointer(currentAttribIndex, element.count, element.type, element.normalized, vbl.getStride(), (void*)offset);
        if (element.isInstancing)
        {
            glVertexAttribDivisor(currentAttribIndex, 1);
        }
        offset += element.count * VertexBufferElement::getSizeOfByte(element.type);
    }

    m_EnabledAttrib += elements.size();
}

void VertexArray::addInstanceData(const VertexBuffer& vb, const unsigned int count)
{
    bind();
    vb.bind();
    glEnableVertexAttribArray(m_EnabledAttrib);
    glVertexAttribPointer(m_EnabledAttrib, count, GL_FLOAT, GL_FALSE, count * sizeof(float), (void*)0);
    // glBindBuffer(GL_ARRAY_BUFFER, 0);
    glVertexAttribDivisor(m_EnabledAttrib, 1);
    vb.unbind();
    m_EnabledAttrib += 1;
}

void VertexArray::bind() const
{
    glBindVertexArray(m_VertexArrayId);
}

void VertexArray::unbind() const
{
    glBindVertexArray(0);
}
