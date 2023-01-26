#pragma once

#include "vertex_buffer.h"
#include "vertex_buffer_layout.h"

class VertexArray
{
private:
    unsigned int m_VertexArrayId;
public:
    VertexArray();
    ~VertexArray();

    void addBuffer(const VertexBuffer& vb, const VertexBufferLayout& vbl) const;
    void bind() const;
    void unbind() const;
};
