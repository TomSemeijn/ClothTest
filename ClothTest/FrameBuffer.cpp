#include "FrameBuffer.h"
#include <glad/glad.h>
#include "Texture.h"

FrameBuffer::FrameBuffer(const glm::vec<2, size_t>& a_size)
	: m_framebuffer(0)
	, m_colorTexture(nullptr)
	, m_size(a_size)
{
	glGenFramebuffers(1, &m_framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
	m_colorTexture = new Texture(a_size);
	m_colorTexture->setAsFrameBufferTexture(*this, GL_COLOR_ATTACHMENT0);
}

FrameBuffer::~FrameBuffer()
{
	if (m_framebuffer)
	{
		glDeleteFramebuffers(1, &m_framebuffer);
	}
	if (m_colorTexture)
	{
		delete m_colorTexture;
	}
}

void FrameBuffer::bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
}

void FrameBuffer::bindWindow()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

Texture& FrameBuffer::getColorTarget()
{
	return *m_colorTexture;
}