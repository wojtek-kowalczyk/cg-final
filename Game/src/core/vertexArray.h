#pragma once

#include "vertexBuffer.h"

class VertexArray
{
private:
	unsigned int m_id;

public:
	VertexArray();
	~VertexArray();

	void AddBuffer(const VertexBuffer& vbuffer, const VertexBufferLayout& layout);
	void Bind() const;
	void Unbind() const;
};