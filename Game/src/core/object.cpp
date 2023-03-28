#include "object.h"
#include <glm/gtc/matrix_transform.hpp>

Object::Object(const std::shared_ptr<Mesh>& mesh, const std::shared_ptr<Material>& material)
	: m_material(material)
{
	m_meshes.push_back(mesh);
}

//Object::Object(const std::vector<std::shared_ptr<Mesh>>& meshes) : m_meshes(meshes)
//{
//}

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
