#pragma once
#include <vector>

typedef unsigned int GLenum;

class VertexLayout
{
private:
	//component structure
	struct LayoutComponent
	{
		GLenum type;
		unsigned int count;
		size_t typeSize;
		bool normalized;
		LayoutComponent(const GLenum a_type, const unsigned int a_count, const size_t a_typeSize, const bool a_normalized) : type(a_type), count(a_count), typeSize(a_typeSize), normalized(a_normalized) {};
	};

	//bound layout
	static VertexLayout* s_boundLayout;

	//stride and components
	unsigned int m_stride{ 0 };
	std::vector<LayoutComponent> m_components;

public:
	//adds a layout component of the specified type
	void addCharComponent(unsigned int a_count, bool a_normalized = false);
	void addIntComponent(unsigned int a_count, bool a_normalized = false);
	void addUCharComponent(unsigned int a_count, bool a_normalized = false);
	void addUIntComponent(unsigned int a_count, bool a_normalized = false);
	void addFloatComponent(unsigned int a_count, bool a_normalized = false);
	void addDoubleComponent(unsigned int a_count, bool a_normalized = false);

	//returns stride
	unsigned int getStride()const;

	//binds this layout
	void bind();
};