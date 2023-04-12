#pragma once

#include "mesh.h"
#include "xmaterial.h"
#include "behaviours/behaviour.h"

#include <glm/glm.hpp>

#include <vector>

class Object
{ 
public:
	glm::vec3 Position	{ 0.0f, 0.0f, 0.0f };
	glm::vec3 Rotation	{ 0.0f, 0.0f, 0.0f };
	glm::vec3 Scale		{ 1.0f, 1.0f, 1.0f };

	Object(const std::pair<std::shared_ptr<Mesh>, std::shared_ptr<xMaterial>>& meshWithMaterialPtr);
	Object(const std::vector<std::pair<std::shared_ptr<Mesh>, std::shared_ptr<xMaterial>>>& meshesWithMaterialPtrs);
	glm::mat4 GetModelMatrix() const;

	void AddBehaviour(std::unique_ptr<Behaviour>& behaviour);
	void Update(float deltaTime);

private:
	std::vector<std::pair<std::shared_ptr<Mesh>, std::shared_ptr<xMaterial>>> m_meshesWithMaterialPtrs;
	std::vector<std::unique_ptr<Behaviour>> m_behaviours; // I can't copy because of this. I can't copy this.

	friend class Scene;
};