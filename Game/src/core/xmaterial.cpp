#include "xMaterial.h"
#include "shaders.h"

xMaterial::xMaterial(glm::vec3 albedo,
	const std::shared_ptr<Shader>& shader,
	const std::vector<std::shared_ptr<Texture>>& diffuseMaps,
	const std::vector<std::shared_ptr<Texture>>& specularMaps,
	float shininess)
	: Albedo(albedo), MainShader(shader), DiffuseMaps(diffuseMaps), SpecularMaps(specularMaps), Shininess(shininess)
{
}

std::shared_ptr<xMaterial> xMaterial::Default()
{
	static auto defaultMat = std::make_shared<xMaterial>(glm::vec3(1, 1, 1), Shaders::basicLit(),
		std::vector<std::shared_ptr<Texture>>{Texture::Default()}, std::vector<std::shared_ptr<Texture>>{Texture::Default()},
		128.0f);
	return defaultMat;
}
 