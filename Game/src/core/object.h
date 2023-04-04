#pragma once

#include "mesh.h"
#include "material.h"

#include <glm/glm.hpp>

#include <vector>

class Object
{
public:
	glm::vec3 Position	{ 0.0f, 0.0f, 0.0f };
	glm::vec3 Rotation	{ 0.0f, 0.0f, 0.0f };
	glm::vec3 Scale		{ 1.0f, 1.0f, 1.0f };

	Object(const std::pair<std::shared_ptr<Mesh>, std::shared_ptr<Material>>& meshWithMaterialPtr);
	Object(const std::vector<std::pair<std::shared_ptr<Mesh>, std::shared_ptr<Material>>>& meshesWithMaterialPtrs);

	glm::mat4 GetModelMatrix() const;

private:
	std::vector<std::pair<std::shared_ptr<Mesh>, std::shared_ptr<Material>>> m_meshesWithMaterialPtrs;

	friend class Scene;
};