#pragma once
#include "vertexArray.h"
#include "vertexBuffer.h"
#include "indexBuffer.h"

#include <memory>

struct Mesh
{
public:
	std::unique_ptr<VertexArray> VAO;
	std::unique_ptr<IndexBuffer> IBO;
	std::unique_ptr<VertexBuffer> VBO;

	// vcount means how many FLOATS is in vertex buffer, not how many Vertex Structs
	Mesh(const float* vertices, unsigned int vCount, const unsigned int* indices, unsigned int iCount, const VertexBufferLayout& layout);
private:

};