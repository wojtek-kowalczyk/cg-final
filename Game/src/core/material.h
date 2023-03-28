#pragma once

#include "shader.h"

#include <glm/glm.hpp>

#include <memory>

class Material
{
public:
	Material(glm::vec3 albedo, const std::shared_ptr<Shader>& shader);

	glm::vec3 Albedo;
	std::shared_ptr<Shader> MainShader;
};