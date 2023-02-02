#pragma once

#include "uniform_buffer_layout.h"

class UniformBuffer
{
private:
    unsigned int m_UniformBufferId;
    int m_UniformBufferOffsetAlign;
    unsigned int m_Size;
public:
    UniformBuffer(); 
    void addBuffer(const UniformBufferLayout& ubl);
    void bindToPoint(unsigned int pointIndex = 0) const;
};
