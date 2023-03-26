#include "renderer.h"

Renderer::Renderer()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL /*GL_LINE*/); // GL_LINE for wireframe
}

void Renderer::Draw(const Mesh& mesh, const Shader& shader) const
{
	mesh.VAO->Bind();
	mesh.IBO->Bind();
	shader.use();
	glDrawElements(m_mode, mesh.IBO->GetCount(), GL_UNSIGNED_INT, 0);
}

void Renderer::Draw(const VertexBuffer& vb, const Shader& shader) const
{
	vb.Bind();
	shader.use();
	glDrawArrays(m_mode, 0, vb.GetSize());
}

void Renderer::Clear(const glm::vec4& color) const
{
	glClearColor(color[0], color[1], color[2], color[3]);
	glClear(m_clearFlags);
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

void Renderer::SetClearFlags(unsigned int bitset)
{
	m_clearFlags = bitset;
}
