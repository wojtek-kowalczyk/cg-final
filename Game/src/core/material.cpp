#include "material.h"

Material::Material(glm::vec3 albedo,
	const std::shared_ptr<Shader>& shader,
	const std::shared_ptr<Texture>& diffuseMap,
	const std::shared_ptr<Texture>& specularMap,
	float shininess)
	: Albedo(albedo), MainShader(shader), DiffuseMap(diffuseMap), SpecularMap(specularMap), Shininess(shininess)
{
}
