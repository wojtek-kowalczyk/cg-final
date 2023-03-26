#include "mesh.h"


Mesh::Mesh(const float* vertices, unsigned int vCount, const unsigned int* indices, unsigned int iCount, const VertexBufferLayout& layout)
{
	VAO = std::make_unique<VertexArray>();
	VAO->Bind();
	IBO = std::make_unique<IndexBuffer>(indices, iCount);
	VBO = std::make_unique<VertexBuffer>(vertices, vCount);
	VAO->AddBuffer(*VBO, layout);
}
