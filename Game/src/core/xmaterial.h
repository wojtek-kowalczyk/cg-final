#pragma once

#include "shader.h"
#include "texture.h"

#include <glm/glm.hpp>

#include <memory> 
#include <vector>

class xMaterial
{
public:
	xMaterial(
		glm::vec3 albedo,
		const std::shared_ptr<Shader>& shader,
		const std::vector<std::shared_ptr<Texture>>& diffuseMaps,
		const std::vector<std::shared_ptr<Texture>>& specularMaps,
		float shininess);

	glm::vec3 Albedo;
	std::vector<std::shared_ptr<Texture>> DiffuseMaps; // TODO : only 1 is supported atm.
	std::vector<std::shared_ptr<Texture>> SpecularMaps; // TODO : only 1 is supported atm.
	std::shared_ptr<Shader> MainShader;
	float Shininess;

	static std::shared_ptr<xMaterial> Default();
};