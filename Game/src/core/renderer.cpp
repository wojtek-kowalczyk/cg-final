#include "renderer.h"

Renderer::Renderer()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL /*GL_LINE*/); // GL_LINE for wireframe
}

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const
{
	va.Bind();
	ib.Bind();
	shader.use();
	glDrawElements(m_mode, ib.GetCount(), GL_UNSIGNED_INT, 0);
}

void Renderer::Draw(const VertexBuffer& vb, const Shader& shader) const
{
	vb.Bind();
	shader.use();
	glDrawArrays(m_mode, 0, vb.GetCount());
}

void Renderer::Clear(const glm::vec4& color) const
{
	glClearColor(color[0], color[1], color[2], color[3]);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::SetMode(Renderer::Mode mode)
{
	switch (mode)
	{
	case Renderer::Mode::Points:
		m_mode = GL_POINTS;
		break;
	
	case Renderer::Mode::Lines:
		m_mode = GL_LINES;
		break;
	
	case Renderer::Mode::Triangles:
		m_mode = GL_TRIANGLES;
		break;
	}
}