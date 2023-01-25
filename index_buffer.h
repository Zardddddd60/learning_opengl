#pragma once

class IndexBuffer
{
private:
    unsigned int m_IndexBufferId;
    // unsigned int m_Count;
public:
    IndexBuffer();
    IndexBuffer(const unsigned int* data, unsigned int size);
    ~IndexBuffer();

    void bind() const;
    void unbind() const;
    void bindData(const unsigned int* data, unsigned int size) const;
};
