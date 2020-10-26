#pragma once
#include <glm/vec3.hpp>

typedef unsigned int GLuint;

class Sphere
{
public:
	Sphere(float a_radius);

	float getRadius()const;
	const glm::vec3& getPos()const;
	
	void setPos(const glm::vec3& a_pos);

	void draw(const class Camera& a_camera)const;

	~Sphere();

private:
	static class Shader* s_shader;
	static class VertexLayout* s_vertexLayout;

	GLuint m_buffer;
	GLuint m_indexBuffer;
	size_t m_indexCount;

	float m_radius;
	glm::vec3 m_pos;

	static void createRenderingResources();

};
