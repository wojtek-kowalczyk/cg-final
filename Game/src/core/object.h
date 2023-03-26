#pragma once

#include <glm/glm.hpp>
#include <vector>

#include "mesh.h"

class Object
{
public:
	glm::vec3 Position	{ 0.0f, 0.0f, 0.0f };
	glm::vec3 Rotation	{ 0.0f, 0.0f, 0.0f };
	glm::vec3 Scale		{ 1.0f, 1.0f, 1.0f };

	Object(const std::weak_ptr<Mesh>& mesh);
	//Object(const std::vector<std::weak_ptr<Mesh>>& meshes);

	glm::mat4 GetModelMatrix();
private:
	std::vector<std::weak_ptr<Mesh>> m_meshes;
	// TODO : material(s)?
};