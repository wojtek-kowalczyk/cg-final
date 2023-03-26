#pragma once

#include <glm/glm.hpp>

class Object
{
	// Object -> transform, mesh[] and material[] -> things that are required to render the opbject as a physical object on screen

public:
	glm::vec3 Position;
	glm::vec3 Rotation;
	glm::vec3 Scale;
private:

};