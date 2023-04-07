#pragma once

#include "mesh.h"
#include "xmaterial.h"

#include <glm/glm.hpp>

#include <vector>

class Object
{
public:
	glm::vec3 Position	{ 0.0f, 0.0f, 0.0f };
	glm::vec3 Rotation	{ 0.0f, 0.0f, 0.0f };
	glm::vec3 Scale		{ 1.0f, 1.0f, 1.0f };

	Object(const std::shared_ptr<Mesh>& mesh, const std::shared_ptr<xMaterial>& material);
	//Object(const std::vector<std::shared_ptr<Mesh>>& meshes);

	glm::mat4 GetModelMatrix() const;
private:
	// will there ever be a case of more meshes than materials or vice versa?
	std::vector<std::shared_ptr<Mesh>> m_meshes;
	std::shared_ptr<xMaterial> m_material;

	friend class Scene;
};