#pragma once

#include "vertexBuffer.h"

class VertexArray
{
public:
	VertexArray();
	~VertexArray();

	void AddBuffer(const VertexBuffer& vbuffer, const VertexBufferLayout& layout);
	void Bind() const;
	void Unbind() const;

private:
	unsigned int m_id;

};