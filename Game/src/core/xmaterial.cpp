#include "xMaterial.h"

xMaterial::xMaterial(glm::vec3 albedo,
	const std::shared_ptr<Shader>& shader,
	const std::vector<std::shared_ptr<Texture>>& diffuseMaps,
	const std::vector<std::shared_ptr<Texture>>& specularMaps,
	float shininess)
	: Albedo(albedo), MainShader(shader), DiffuseMaps(diffuseMaps), SpecularMaps(specularMaps), Shininess(shininess)
{
}
