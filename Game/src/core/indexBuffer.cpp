#include "indexBuffer.h"
#include "glad/glad.h"

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count) :
	m_count (count)
{
	// we could assert sizeof(unsigned int) == sizeof(GLuint) or use GLuint or idk
	glGenBuffers(1, &m_rendererId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW);
	// TODO : consider if using hardcoded unsigned int type is a good idea, maybe it could just use size like the other (what we need count for?)
}

IndexBuffer::~IndexBuffer()
{
	glDeleteBuffers(1, &m_rendererId);
}

void IndexBuffer::Bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, m_rendererId);
}

void IndexBuffer::Unbind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
