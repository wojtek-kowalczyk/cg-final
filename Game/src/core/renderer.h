#pragma once

#include <glm/vec4.hpp>

#include "indexBuffer.h"
#include "vertexArray.h"
#include "shader.h"
#include "mesh.h"

class Renderer
{
public:
	enum class Mode { Points, Lines, Triangles };

	Renderer();
	void Draw(const Mesh& mesh, const Shader& shader) const;
	void Draw(const VertexBuffer& vb, const Shader& shader) const;
	void Clear(const glm::vec4& color = { 0.0f, 0.0f, 0.0f, 1.0f }) const; // TODO : we could set the clear color before and not pass it here, just like clear flags
	void SetMode(Renderer::Mode mode);
	void SetClearFlags(unsigned int bitset);

private:
	unsigned int m_mode{ GL_TRIANGLES };
	unsigned int m_clearFlags{ GL_COLOR_BUFFER_BIT };
};
