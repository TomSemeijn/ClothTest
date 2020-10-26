#include "Texture.h"
#include <glad/glad.h>
#include <stb_image.h>
#include "FrameBuffer.h"

Texture::Texture(const std::string& a_filename)
	: m_texture(0)
{
	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_2D, m_texture);

	int width, height, channels;
	unsigned char* data = stbi_load(a_filename.c_str(), &width, &height, &channels, 4);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	stbi_image_free(data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

Texture::Texture(const glm::vec<2, size_t>& a_size)
	: m_texture(0)
{
	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_2D, m_texture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, a_size.x, a_size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

Texture::~Texture()
{
	if (m_texture)
	{
		glDeleteTextures(1, &m_texture);
	}
}

void Texture::bind(unsigned int a_slot)
{
	glActiveTexture(GL_TEXTURE0 + a_slot);
	glBindTexture(GL_TEXTURE_2D, m_texture);
}

void Texture::setAsFrameBufferTexture(FrameBuffer& a_frameBuffer, unsigned int a_attachment)
{
	a_frameBuffer.bind();
	glFramebufferTexture2D(GL_FRAMEBUFFER, a_attachment, GL_TEXTURE_2D, m_texture, 0);
}
