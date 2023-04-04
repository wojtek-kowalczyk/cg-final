#include "object.h"

#include <glm/gtc/matrix_transform.hpp>

Object::Object(const std::pair<std::shared_ptr<Mesh>, std::shared_ptr<Material>>& meshWithMaterialPtr)
{
	m_meshesWithMaterialPtrs.push_back(meshWithMaterialPtr);
}

Object::Object(const std::vector<std::pair<std::shared_ptr<Mesh>, std::shared_ptr<Material>>>& meshesWithMaterialPtrs)
{
	m_meshesWithMaterialPtrs.insert(m_meshesWithMaterialPtrs.end(), meshesWithMaterialPtrs.begin(), meshesWithMaterialPtrs.end());
}

glm::mat4 Object::GetModelMatrix() const
{
	glm::mat4 model{ 1.0f };
	
	// TODO : pivot handling

	model = glm::translate(model, Position);
	model = glm::rotate(model, glm::radians(Rotation.x), glm::vec3(1, 0, 0));
	model = glm::rotate(model, glm::radians(Rotation.y), glm::vec3(0, 1, 0));
	model = glm::rotate(model, glm::radians(Rotation.z), glm::vec3(0, 0, 1));
	model = glm::scale(model, Scale);

	return model;
}
