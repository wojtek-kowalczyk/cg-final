#include <glad/glad.h>
#include "vertexBuffer.h"

VertexBuffer::VertexBuffer(const float* data, unsigned int count) : m_count { count }
{
	glGenBuffers(1, &m_id);
	glBindBuffer(GL_ARRAY_BUFFER, m_id);
	// TODO : this will sometimes (1/5) throw Unhandled exception, 
	// access vialoation trying to read data at 0x0... with NON ZERO address. Investigate
	// I'm reading that it might be solved by upgrading gpu drivers. It is said to happen more on AMD GPUs
	// and I realized that this program is running on my integrated GPU (AMD) rather than on the dedicated one (NVidia)
	glBufferData(GL_ARRAY_BUFFER, count * sizeof(float), (const void*)data, GL_STATIC_DRAW);
}

VertexBuffer::~VertexBuffer()
{
	glDeleteBuffers(1, &m_id);
}

void VertexBuffer::Bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, m_id);
}

void VertexBuffer::Unbind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBufferLayout::Add(const Element& element)
{
	m_elements.push_back(element);
}

unsigned int VertexBufferLayout::GetStride() const
{
	unsigned int stride = 0;

	for (int i = 0; i < m_elements.size(); ++i)
	{
		stride += m_elements[i].amount * GetSizeForType(m_elements[i].type);
	}

	return stride;
}

const std::vector<VertexBufferLayout::Element>& VertexBufferLayout::GetElements() const
{
	return m_elements;
}

unsigned int VertexBufferLayout::GetSizeForType(unsigned int type) const
{
	switch (type)
	{
		case GL_FLOAT:	return sizeof(float);
		default:		return 0;
	}
}
