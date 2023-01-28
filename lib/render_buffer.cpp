#include <glad/glad.h>
#include "render_buffer.h"

RenderBuffer::RenderBuffer()
{
    glGenRenderbuffers(1, &m_RenderBufferId);
}

RenderBuffer::~RenderBuffer()
{
    // glDeleteRenderbuffers(1, &m_RenderBufferId);
}

void RenderBuffer::attach(unsigned int width, unsigned int height)
{
    glBindRenderbuffer(GL_RENDERBUFFER, m_RenderBufferId);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RenderBufferId);
}
