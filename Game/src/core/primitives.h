#include "mesh.h"	

#include <glm/glm.hpp>

#include <memory>

namespace Primitives
{
	std::shared_ptr<Mesh> Cube();
	std::shared_ptr<Mesh> Plane();
	// Sphere is different cuz I need to satisfy the assignment: one model made of 4 primitive meshes.
	std::shared_ptr<Mesh> Sphere(glm::vec3 offset = glm::vec3(0, 0, 0), glm::vec3 scale = glm::vec3(1, 1, 1));
};