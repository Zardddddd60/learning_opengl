#include "texture_frame_buffer.h"
#include <cstddef>
#include <glad/glad.h>

TextureFrameBuffer::TextureFrameBuffer()
{
    glGenTextures(1, &m_TextureFrameBufferId);
    // glBindFramebuffer(GL_FRAMEBUFFER, m_TextureFrameBufferId);
}

TextureFrameBuffer::~TextureFrameBuffer()
{
    // 本质上是应该做删除的…
    // glDeleteFramebuffers(1, &m_TextureFrameBufferId);
}

void TextureFrameBuffer::attach(unsigned int width, unsigned int height) const
{
    glBindTexture(GL_TEXTURE_2D, m_TextureFrameBufferId);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_TextureFrameBufferId, 0);
}

void TextureFrameBuffer::bind(unsigned int slot) const
{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, m_TextureFrameBufferId);
}
