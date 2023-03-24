#pragma once

#include "indexBuffer.h"
#include "vertexArray.h"
#include "shader.h"
#include "glm/vec4.hpp"

class Renderer
{
public:
	enum class Mode { Points, Lines, Triangles };
		
	Renderer();
	void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
	void Draw(const VertexBuffer& vb, const Shader& shader) const;
	void Clear(const glm::vec4& color = { 0.0f, 0.0f, 0.0f, 1.0f }) const;
	void SetMode(Renderer::Mode mode);

private:
	unsigned int m_mode {GL_TRIANGLES};
};
