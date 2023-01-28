#include <iostream>
#include <glad/glad.h>
#include "frame_buffer.h"

FrameBuffer::FrameBuffer()
{
    glGenFramebuffers(1, &m_FrameBufferId);
}
FrameBuffer::~FrameBuffer()
{
    // glDeleteFramebuffers(1, &m_FrameBufferId);
}

void FrameBuffer::bind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBufferId);
}

void FrameBuffer::unbind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

bool FrameBuffer::checkComplete() const
{
    bool isComplete = glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
    if (!isComplete)
    {
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    }
    return isComplete;
}
