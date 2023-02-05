#pragma once

#include "vertex_buffer.h"
#include "vertex_buffer_layout.h"

class VertexArray
{
private:
    unsigned int m_VertexArrayId;
    unsigned int m_EnabledAttrib = 0;
public:
    VertexArray();
    ~VertexArray();

    void addBuffer(const VertexBuffer& vb, const VertexBufferLayout& vbl);
    void addInstanceData(const VertexBuffer& vb, const unsigned int count);
    void bind() const;
    void unbind() const;
};
