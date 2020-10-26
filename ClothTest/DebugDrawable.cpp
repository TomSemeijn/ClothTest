#include "DebugDrawable.h"
#include <glad/glad.h>
#include <glm/mat4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include "Shader.h"
#include "VertexLayout.h"
#include "Camera.h"

Shader* DebugDrawable::s_shader = nullptr;
VertexLayout* DebugDrawable::s_layout = nullptr;

DebugDrawable::DebugDrawable()
{
	if (!s_shader)
	{
		createRenderingResources();
	}
	glGenBuffers(1, &m_buffer);
}

DebugDrawable& DebugDrawable::operator=(DebugDrawable&& a_toMove)
{
	m_buffer = a_toMove.m_buffer;
	a_toMove.m_buffer = 0;
	return *this;
}

DebugDrawable::~DebugDrawable()
{
	if (m_buffer)
	{
		glDeleteBuffers(1, &m_buffer);
	}
}

void DebugDrawable::drawData(const Camera& a_camera, GLenum a_type, size_t a_size, void* a_data, size_t a_vertexCount)const
{
	s_shader->bind();
	s_shader->setUniform("u_vp", a_camera.getView() * a_camera.getProjection());

	glBindBuffer(GL_ARRAY_BUFFER, m_buffer);
	glBufferData(GL_ARRAY_BUFFER, a_size, a_data, GL_DYNAMIC_DRAW);

	s_layout->bind();

	glDrawArrays(a_type, 0, a_vertexCount);
}

void DebugDrawable::createRenderingResources()
{
	s_shader = new Shader(

		//vertex shader
		"#version 330 core\n"
		""
		"layout(location = 0) in vec3 a_position;\n"
		"layout(location = 1) in vec3 a_color;\n"
		""
		"uniform mat4 u_vp;\n"
		""
		"out vec3 v_color;\n"
		""
		"void main()\n"
		"{\n"
		"	gl_Position = u_vp * vec4(a_position, 1.0);\n"
		"	v_color = a_color;\n"
		"}\n"

		,

		//pixel shader
		"#version 330 core\n"
		""
		"in vec3 v_color;\n"
		""
		"out vec4 outColor;\n"
		""
		"void main()\n"
		"{\n"
		"	outColor = vec4(v_color, 1.0);\n"
		"}\n"

	);

	s_shader->bind();

	s_layout = new VertexLayout();
	s_layout->addFloatComponent(3); //pos
	s_layout->addFloatComponent(3); //color
}
