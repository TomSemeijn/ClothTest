#pragma once
#include <string>
#include <glm/detail/type_vec2.hpp>

typedef unsigned int GLuint;
class FrameBuffer;

class Texture
{
public:
	Texture(const std::string& a_filename);
	Texture(const glm::vec<2, size_t>& a_size);
	Texture(const Texture&) = delete;
	Texture& operator=(const Texture&) = delete;
	~Texture();

	void bind(unsigned int a_slot = 0);

	void setAsFrameBufferTexture(FrameBuffer& a_frameBuffer, unsigned int a_attachment);

private:
	GLuint m_texture;
};