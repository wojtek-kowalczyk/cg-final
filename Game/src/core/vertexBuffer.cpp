#include "vertexBuffer.h"
#include "glad/glad.h"

VertexBuffer::VertexBuffer(const void* data, unsigned int size) : m_count { size }
{
	glGenBuffers(1, &m_rendererId);
	glBindBuffer(GL_ARRAY_BUFFER, m_rendererId);
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

VertexBuffer::~VertexBuffer()
{
	glDeleteBuffers(1, &m_rendererId);
}

void VertexBuffer::Bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, m_rendererId);
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
		stride += m_elements[i].amount * sizeof(GetSizeForType(m_elements[i].type));
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
		case GL_FLOAT:	return 4;
		default:		return 0;
	}
}
