#include "vertexArray.h"
#include "glad/glad.h"

VertexArray::VertexArray()
{
	glGenVertexArrays(1, &m_id);
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &m_id);
}

void VertexArray::AddBuffer(const VertexBuffer& vbuffer, const VertexBufferLayout& layout)
{
	Bind();
	vbuffer.Bind();
	const auto& elements = layout.GetElements();
	unsigned int offset = 0;
	for (int i = 0; i < elements.size(); ++i)
	{
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, elements[i].amount, elements[i].type, elements[i].normalized,
			layout.GetStride(), (const void*)(static_cast<uint64_t>(offset) * layout.GetSizeForType(elements[i].type)));
		offset += elements[i].amount;
	}
}

void VertexArray::Bind() const
{
	glBindVertexArray(m_id);
}

void VertexArray::Unbind() const
{
	glBindVertexArray(0);
}
