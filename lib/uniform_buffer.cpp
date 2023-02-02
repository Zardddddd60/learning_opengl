#include <glad/glad.h>
#include "uniform_buffer.h"
#include "glm/gtc/type_ptr.hpp"

UniformBuffer::UniformBuffer()
{
    glGetIntegerv(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT, &m_UniformBufferOffsetAlign);
    glGenBuffers(1, &m_UniformBufferId);
    m_Size = 0;
}

void UniformBuffer::addBuffer(const UniformBufferLayout& ubl)
{   
    // 先算总体大小，计算glBufferData申请的大小
    unsigned int bufferSize = 0;
    auto& elements = ubl.getElements();
    for (unsigned int i = 0; i < elements.size(); i ++)
    {
        // glBufferSubData
        bufferSize += elements[i].size;
    }
    m_Size = bufferSize;

    glBindBuffer(GL_UNIFORM_BUFFER, m_UniformBufferId);
    // 申请空间
    glBufferData(GL_UNIFORM_BUFFER, m_Size, NULL, GL_STATIC_DRAW);
    unsigned int offset = 0;
    for (unsigned int i = 0; i < elements.size(); i ++)
    {
        // glBufferSubData
        glBufferSubData(GL_UNIFORM_BUFFER, offset, elements[i].size, elements[i].data);
        offset += elements[i].size;
    }
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

}

void UniformBuffer::bindToPoint(unsigned int pointIndex) const
{
    // 据说是要m_UniformBufferOffsetAlign的倍数
    const int times = static_cast<int>(m_Size / m_UniformBufferOffsetAlign);
    const int offset = m_Size % m_UniformBufferOffsetAlign == 0 ? 0 : 1;
    // 试了一下就算不是m_UniformBufferOffsetAlign的倍数也可以
    glBindBufferRange(GL_UNIFORM_BUFFER, pointIndex, m_UniformBufferId, 0, (times + offset) * m_UniformBufferOffsetAlign);
}
