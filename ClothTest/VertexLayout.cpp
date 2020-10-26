#include "VertexLayout.h"
#include <glad/glad.h>

//adds a char layout component
void VertexLayout::addCharComponent(unsigned int a_count, bool a_normalized)
{
	m_components.emplace_back(GL_BYTE, a_count, sizeof(char), a_normalized); //const GLenum a_type, const unsigned int a_count, const unsigned int a_typeSize, const bool a_normalized
	m_stride += a_count * sizeof(char);
}

//adds an int layout component
void VertexLayout::addIntComponent(unsigned int a_count, bool a_normalized)
{
	m_components.emplace_back(GL_INT, a_count, sizeof(int), a_normalized);
	m_stride += a_count * sizeof(int);
}

//adds an unsigned char layout component
void VertexLayout::addUCharComponent(unsigned int a_count, bool a_normalized)
{
	m_components.emplace_back(GL_UNSIGNED_INT, a_count, sizeof(unsigned int), a_normalized);
	m_stride += a_count * sizeof(unsigned int);
}

//adds an unsigned int layout component
void VertexLayout::addUIntComponent(unsigned int a_count, bool a_normalized)
{
	m_components.emplace_back(GL_UNSIGNED_BYTE, a_count, sizeof(unsigned char), a_normalized);
	m_stride += a_count * sizeof(unsigned char);
}

//adds a float layout component
void VertexLayout::addFloatComponent(unsigned int a_count, bool a_normalized)
{
	m_components.emplace_back(GL_FLOAT, a_count, sizeof(float), a_normalized);
	m_stride += a_count * sizeof(float);
}

//adds a double layout component
void VertexLayout::addDoubleComponent(unsigned int a_count, bool a_normalized)
{
	m_components.emplace_back(GL_DOUBLE, a_count, sizeof(double), a_normalized);
	m_stride += a_count * sizeof(double);
}

//returns stride
unsigned int VertexLayout::getStride()const
{
	return m_stride;
}

//binds this layout
void VertexLayout::bind()
{
	glEnableVertexAttribArray(0);
	size_t offset = 0;
	for (unsigned int k = 0; k < m_components.size(); k++)
	{
		auto& comp = m_components[k];
		glVertexAttribPointer(k, comp.count, GL_FLOAT, comp.normalized ? GL_TRUE : GL_FALSE, m_stride, reinterpret_cast<const void*>(offset));
		glEnableVertexAttribArray(k);
		offset += comp.typeSize * comp.count;
	}
}