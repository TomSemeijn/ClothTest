#include <glm/detail/type_vec2.hpp>

typedef unsigned int GLuint;
class Texture;

class FrameBuffer
{
public:
	FrameBuffer(const glm::vec<2, size_t>& a_size);
	~FrameBuffer();

	void bind();
	static void bindWindow();

	Texture& getColorTarget();

	const glm::vec<2, size_t>& getSize()const { return m_size; };

private:
	GLuint m_framebuffer;
	Texture* m_colorTexture;
	glm::vec<2, size_t> m_size;

};