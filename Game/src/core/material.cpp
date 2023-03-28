#include "material.h"

Material::Material(glm::vec3 albedo, const std::shared_ptr<Shader>& shader)
	: Albedo(albedo), MainShader(shader)
{
}
