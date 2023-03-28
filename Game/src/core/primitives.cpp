#include "primitives.h"

// TODO : do I need entire 3k lines glad header when I only want the 0x1406 value of GL_FLOAT? or will the compiler strip that anyway?
#include <glad/glad.h>

std::shared_ptr<Mesh> Primitives::Cube()
{
	static constexpr float vertices[] = {
		// front		
		-1.0, -1.0,  1.0,
		 1.0, -1.0,  1.0,
		 1.0,  1.0,  1.0,
		-1.0,  1.0,  1.0,
		// back			
		-1.0, -1.0, -1.0,
		 1.0, -1.0, -1.0,
		 1.0,  1.0, -1.0,
		-1.0,  1.0, -1.0,
	};

	static constexpr unsigned int indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		2, 1, 6,
		6, 1, 5,
		// back
		6, 7, 4,
		4, 5, 6,
		// left
		7, 4, 0,
		0, 7, 3,
		//top
		7, 3, 2,
		2, 6, 7,
		//bottom
		4, 0, 1,
		4, 1, 5,
	};

	static std::shared_ptr<Mesh> cube = nullptr;

    if (!cube)
    {
		VertexBufferLayout layout;
		layout.Add(VertexBufferLayout::Element{ 3, GL_FLOAT }); // position

        cube = std::make_shared<Mesh>(vertices, sizeof(vertices) / sizeof(float), 
			indices, sizeof(indices) / sizeof(unsigned int), layout);
    }

    return cube;
}

std::shared_ptr<Mesh> Primitives::Plane()
{
	static constexpr float vertices[] = {
		-1.0f, 0.0f, -1.0f,
		+1.0f, 0.0f, -1.0f,
		+1.0f, 0.0f, +1.0f,
		-1.0f, 0.0f, +1.0f,
	};

	static constexpr unsigned int indices[] = {
		0, 1, 2,
		2, 0, 3,
	};

	static std::shared_ptr<Mesh> plane = nullptr;

	if (!plane)
	{
		VertexBufferLayout layout;
		layout.Add(VertexBufferLayout::Element{ 3, GL_FLOAT }); // position

		plane = std::make_shared<Mesh>(vertices, sizeof(vertices) / sizeof(float),
			indices, sizeof(indices) / sizeof(unsigned int), layout);
	}

	return plane;
}
