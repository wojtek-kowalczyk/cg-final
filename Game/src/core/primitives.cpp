#include "primitives.h"

// TODO : do I need entire 3k lines glad header when I only want the 0x1406 value of GL_FLOAT? or will the compiler strip that anyway?
#include <glad/glad.h>
#include <glm/gtc/constants.hpp>

std::shared_ptr<Mesh> Primitives::Cube()
{
	float vertices123[] = {
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
	};
	
	static constexpr float vertices[] = {
		// positions			// normals
		-1.0f, -1.0f, +1.0f,	-0.0000, -1.0000, -0.0000, // n5, // 0
		-1.0f, +1.0f, +1.0f,	-0.0000, +1.0000, -0.0000, // n6, // 1
		-1.0f, -1.0f, -1.0f,	-0.0000, -1.0000, -0.0000, // n5, // 2
		-1.0f, +1.0f, -1.0f,	-0.0000, +1.0000, -0.0000, // n6, // 3

		+1.0f, -1.0f, +1.0f,	-0.0000, -1.0000, -0.0000, // n5, // 4
		+1.0f, +1.0f, +1.0f,	-0.0000, +1.0000, -0.0000, // n6, // 5
		+1.0f, -1.0f, -1.0f,	-0.0000, -1.0000, -0.0000, // n5, // 6
		+1.0f, +1.0f, -1.0f,	-0.0000, +1.0000, -0.0000, // n6, // 7
		
		-1.0f, -1.0f, -1.0f,	-1.0000, -0.0000, -0.0000, // n1, // 8
		-1.0f, -1.0f, -1.0f,	-0.0000, -0.0000, -1.0000, // n2, // 9
		-1.0f, -1.0f, +1.0f,	-1.0000, -0.0000, -0.0000, // n1, // 10
		-1.0f, -1.0f, +1.0f,	-0.0000, -0.0000, -1.0000, // n4, // 11
		
		-1.0f, +1.0f, +1.0f,	-1.0000, -0.0000, -0.0000, // n1, // 12
		-1.0f, +1.0f, +1.0f,	-0.0000, -0.0000, -1.0000, // n4, // 13
		-1.0f, +1.0f, -1.0f,	-1.0000, -0.0000, -0.0000, // n1, // 14
		-1.0f, +1.0f, -1.0f,	-0.0000, -0.0000, -1.0000, // n2, // 15
		
		+1.0f, -1.0f, -1.0f,	-0.0000, -0.0000, -1.0000, // n2, // 16
		+1.0f, -1.0f, -1.0f,	+1.0000, -0.0000, -0.0000, // n3, // 17
		+1.0f, +1.0f, -1.0f,	-0.0000, -0.0000, -1.0000, // n2, // 18
		+1.0f, +1.0f, -1.0f,	+1.0000, -0.0000, -0.0000, // n3, // 19
		
		+1.0f, -1.0f, +1.0f,	+1.0000, -0.0000, -0.0000, // n3, // 20
		+1.0f, -1.0f, +1.0f,	-0.0000, -0.0000, -1.0000, // n4, // 21
		+1.0f, +1.0f, +1.0f,	+1.0000, -0.0000, -0.0000, // n3, // 22
		+1.0f, +1.0f, +1.0f,	-0.0000, -0.0000, -1.0000, // n4, // 23
	};

	static constexpr unsigned int indices[] = {
		13-1, 9-1,  11-1, 
		16-1, 17-1, 10-1, 
		20-1, 21-1, 18-1, 
		24-1, 12-1, 22-1, 
		7-1,  1-1,  3-1, 
		4-1,  6-1,  8-1, 
		13-1, 15-1, 9-1, 
		16-1, 19-1, 17-1, 
		20-1, 23-1, 21-1, 
		24-1, 14-1, 12-1, 
		7-1,  5-1,  1-1, 
		4-1,  2-1,  6-1, 
	};


	static std::shared_ptr<Mesh> cube = nullptr;

	if (!cube)
	{
		VertexBufferLayout layout;
		layout.Add(VertexBufferLayout::Element{ 3, GL_FLOAT }); // position
		layout.Add(VertexBufferLayout::Element{ 3, GL_FLOAT }); // normal

		cube = std::make_shared<Mesh>(vertices, sizeof(vertices) / sizeof(float),
			indices, sizeof(indices) / sizeof(unsigned int), layout);
	}

	return cube;
}

std::shared_ptr<Mesh> Primitives::Plane()
{
	static constexpr float vertices[] = {
		// positions			// normals
		-1.0f, 0.0f, -1.0f,		0.0f, 1.0f, 0.0f,
		+1.0f, 0.0f, -1.0f,		0.0f, 1.0f, 0.0f,
		+1.0f, 0.0f, +1.0f,		0.0f, 1.0f, 0.0f,
		-1.0f, 0.0f, +1.0f,		0.0f, 1.0f, 0.0f,
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
		layout.Add(VertexBufferLayout::Element{ 3, GL_FLOAT }); // normals

		plane = std::make_shared<Mesh>(vertices, sizeof(vertices) / sizeof(float),
			indices, sizeof(indices) / sizeof(unsigned int), layout);
	}

	return plane;
}

static float lerp(float a, float b, float t)
{
	return a + t * (b - a);
}

struct SphereVertex { float x, y, z, nx, ny, nz, s, t; };
/*
static void buildSphereMesh(std::vector<SphereVertex>& vertexBuffer, std::vector<unsigned int>& indexBuffer, float radius, unsigned int samples)
{
	float x, y, z;
	float U, V; // u rows, v columns

	// generate points
	for (int i = 0; i <= samples; ++i)
	{
		constexpr float pi = glm::pi<float>();
		V = lerp(pi / 2.0f, -pi / 2.0f, (float)i / samples);
		z = radius * sinf(V);

		for (int j = 0; j <= samples; ++j)
		{
			U = lerp(0.0f, pi * 2.0f, (float)j / samples);
			x = radius * cosf(V) * cosf(U);
			y = radius * cosf(V) * sinf(U);
			vertexBuffer.push_back({ x, y, z, 0, 0, 0, (float)j / samples, (float)i / samples });
		}
	}

	// triangulate
	for (int i = 0; i < samples; ++i)
	{
		int k1 = (samples + 1) * (i + 0);
		int k2 = (samples + 1) * (i + 1);

		for (int j = 0; j < samples; ++j, ++k1, ++k2)
		{
			if (i != 0)
			{
				indexBuffer.push_back(k1);
				indexBuffer.push_back(k2);
				indexBuffer.push_back(k1 + 1);
			}

			if (i != (samples - 1))
			{
				indexBuffer.push_back(k1 + 1);
				indexBuffer.push_back(k2);
				indexBuffer.push_back(k2 + 1);
			}
		}
	}
}
*/

// adapted from https://gist.github.com/Pikachuxxxx/5c4c490a7d7679824e0e18af42918efc
void GenerateSphereSmooth(std::vector<SphereVertex>& vertices, std::vector<unsigned int>& indices, int radius, int latitudes, int longitudes)
{
	float nx, ny, nz, lengthInv = 1.0f / radius;    // normal

	constexpr float pi = glm::pi<float>();

	float deltaLatitude = pi / latitudes;
	float deltaLongitude = 2 * pi / longitudes;
	float latitudeAngle;
	float longitudeAngle;

	// Compute all vertices first except normals
	for (int i = 0; i <= latitudes; ++i)
	{
		latitudeAngle = pi / 2 - i * deltaLatitude; /* Starting -pi/2 to pi/2 */
		float xy = radius * cosf(latitudeAngle);    /* r * cos(phi) */
		float z = radius * sinf(latitudeAngle);     /* r * sin(phi )*/

		/*
		 * We add (latitudes + 1) vertices per longitude because of equator,
		 * the North pole and South pole are not counted here, as they overlap.
		 * The first and last vertices have same position and normal, but
		 * different tex coords.
		 */
		for (int j = 0; j <= longitudes; ++j)
		{
			longitudeAngle = j * deltaLongitude;

			SphereVertex vertex;
			vertex.x = xy * cosf(longitudeAngle);   /* x = r * cos(phi) * cos(theta)  */
			vertex.y = xy * sinf(longitudeAngle);   /* y = r * cos(phi) * sin(theta) */
			vertex.z = z;                           /* z = r * sin(phi) */
			vertex.s = (float)j / longitudes;       /* s */
			vertex.t = (float)i / latitudes;        /* t */

			// normalized vertex normal
			vertex.nx = vertex.x * lengthInv;
			vertex.ny = vertex.y * lengthInv;
			vertex.nz = vertex.z * lengthInv;
			
			vertices.push_back(vertex);
		}
	}

	/*
	 *  Indices
	 *  k1--k1+1
	 *  |  / |
	 *  | /  |
	 *  k2--k2+1
	 */
	unsigned int k1, k2;
	for (int i = 0; i < latitudes; ++i)
	{
		k1 = i * (longitudes + 1);
		k2 = k1 + longitudes + 1;
		// 2 Triangles per latitude block excluding the first and last longitudes blocks
		for (int j = 0; j < longitudes; ++j, ++k1, ++k2)
		{
			if (i != 0)
			{
				indices.push_back(k1);
				indices.push_back(k2);
				indices.push_back(k1 + 1);
			}

			if (i != (latitudes - 1))
			{
				indices.push_back(k1 + 1);
				indices.push_back(k2);
				indices.push_back(k2 + 1);
			}
		}
	}
}

std::shared_ptr<Mesh> Primitives::Sphere()
{
	static std::shared_ptr<Mesh> sphere = nullptr;

	if (sphere == nullptr)
	{
		std::vector<SphereVertex> vertices;
		std::vector<unsigned int> indices;
		GenerateSphereSmooth(vertices, indices, 1.0f, 8.0f, 16.0f);
		VertexBufferLayout layout;
		layout.Add({ 3, GL_FLOAT }); // pos
		layout.Add({ 3, GL_FLOAT }); // normal
		layout.Add({ 2, GL_FLOAT }); // uv
		sphere = std::make_shared<Mesh>((float*)vertices.data(), vertices.size() * (sizeof(SphereVertex) / sizeof(float)),
			indices.data(), indices.size(), layout);
	}

	return sphere;
}
