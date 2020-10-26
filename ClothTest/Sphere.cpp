#include "Sphere.h"
#include <glad/glad.h>
#include <glm/ext/matrix_transform.hpp>
#include "SphereGen/SphereGenerator.h"
#include "Shader.h"
#include "VertexLayout.h"
#include "Camera.h"

Shader* Sphere::s_shader = nullptr;
VertexLayout* Sphere::s_vertexLayout = nullptr;

Sphere::Sphere(float a_radius)
	: m_buffer(0)
	, m_indexBuffer(0)
	, m_indexCount(0)
	, m_radius(a_radius)
	, m_pos(0, 0, 0)
{
	SphereGenerator generator(a_radius);

	glGenBuffers(1, &m_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_buffer);
	glBufferData(GL_ARRAY_BUFFER, generator.getInterleavedVertexSize(), generator.getInterleavedVertices(), GL_STATIC_DRAW);

	glGenBuffers(1, &m_indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, generator.getIndexSize(), generator.getIndices(), GL_STATIC_DRAW);
	m_indexCount = generator.getIndexCount();

	createRenderingResources();
}

void Sphere::createRenderingResources()
{
	if (!s_shader)
	{
		s_shader = new Shader(
			"#version 330 core\n"
			""
			"in vec3 a_pos;"
			"in vec3 a_normal;"
			"in vec2 a_texCoord;"
			""
			"uniform mat4 u_mvp;"
			""
			"out vec3 v_worldPos;"
			"out vec3 v_normal;"
			""
			"void main()"
			"{"
			"	gl_Position = u_mvp * vec4(a_pos, 1.0);"
			"	v_worldPos = a_pos;"
			"	v_normal = vec3(a_normal.x, -a_normal.y, a_normal.z);"
			"}"

			,

			"#version 330 core\n"
			""
			"in vec3 v_worldPos;"
			"in vec3 v_normal;"
			""
			"uniform vec3 u_color;"
			""
			"out vec4 outColor;"
			""
			"void main()"
			"{"
			"   vec3 lightPos = vec3(10.0, 20.0, -5.0);"
			"   vec3 lightDir = normalize(v_worldPos - lightPos);"
			"   float lightIntensity = clamp(dot(v_normal, lightDir), 0.2, 1.0);"
			"	outColor = vec4(u_color * lightIntensity, 1.0);"
			"}"
		);

		s_shader->setUniform("u_color", glm::vec3(0.5f, 1.f, 0.5f));

		s_vertexLayout = new VertexLayout();
		s_vertexLayout->addFloatComponent(3); //pos
		s_vertexLayout->addFloatComponent(3); //normal
		s_vertexLayout->addFloatComponent(2); //texcoord
	}
}

void Sphere::draw(const class Camera& a_camera)const
{
	s_shader->bind();
	s_shader->setUniform("u_mvp", a_camera.getView() * a_camera.getProjection() * glm::translate(glm::identity<glm::mat4x4>(), m_pos));

	glBindBuffer(GL_ARRAY_BUFFER, m_buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);

	s_vertexLayout->bind();

	glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, (void*)0);
}

float Sphere::getRadius()const
{
	return m_radius;
}

const glm::vec3& Sphere::getPos()const
{
	return m_pos;
}

void Sphere::setPos(const glm::vec3& a_pos)
{
	m_pos = a_pos;
}

Sphere::~Sphere()
{
	glDeleteBuffers(1, &m_buffer);
	glDeleteBuffers(1, &m_indexBuffer);
}