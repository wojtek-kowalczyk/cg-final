#pragma once

#include "shader.h"
#include "texture.h"

#include <glm/glm.hpp>

#include <memory>

class Material
{
public:
	Material(
		glm::vec3 albedo,
		const std::shared_ptr<Shader>& shader,
		const std::shared_ptr<Texture>& diffuseMap, 
		const std::shared_ptr<Texture>& specularMap,
		float shininess);

	glm::vec3 Albedo;
	std::shared_ptr<Texture> DiffuseMap;
	std::shared_ptr<Texture> SpecularMap;
	std::shared_ptr<Shader> MainShader;
	float Shininess;

	// TODO : const? static? STATIC CONST? lord help me
	const int DiffuseTextureSlot = 0;
	const int SpecularTextureSlot = 1;
};