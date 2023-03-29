#include "mesh.h"	

#include <memory>

namespace Primitives
{
	std::shared_ptr<Mesh> Cube();
	std::shared_ptr<Mesh> Plane();
	std::shared_ptr<Mesh> Sphere();
};