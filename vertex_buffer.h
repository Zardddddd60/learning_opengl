#pragma once

class VertexBuffer
{
private:
    unsigned int m_VertextBufferId;
public:
    VertexBuffer();
    VertexBuffer(const void* data, unsigned int size);
    ~VertexBuffer();

    void bind() const;
    void unbind() const;
    void bindData(const void* data, unsigned int size) const;
};
