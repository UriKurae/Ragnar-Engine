#pragma once

#include "VertexBuffer.h"

typedef unsigned char GLubyte;

namespace RCube
{
	void CreateCube(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices)
	{
		/*vertices =
		{
			{-0.5f, 0.5f, 0.5f},
			{-0.5f, -0.5f, 0.5f},
			{0.5f, -0.5f, 0.5f},
			{0.5f, 0.5f, 0.5f},

			{0.5f, -0.5f, -0.5f},
			{0.5f, 0.5f, -0.5f},
			{-0.5f, -0.5f, -0.5f},
			{-0.5f, 0.5f, -0.5f},
		};*/

		Vertex vertex;

		// 0
		vertex.position = { -0.5f,  0.5f,  0.5f };
		vertex.texCoords = { 0,0 };
		vertex.normal = { vertex.position };
		vertices.emplace_back(vertex);

		// 1
		vertex.position = { -0.5f, -0.5f,  0.5f };
		vertex.texCoords = { 0,1 };
		vertex.normal = { vertex.position };
		vertices.emplace_back(vertex);
		// 2
		vertex.position = { 0.5f, -0.5f,  0.5f };
		vertex.texCoords = { 1,1 };
		vertex.normal = { vertex.position };
		vertices.emplace_back(vertex);
		// 3
		vertex.position = { 0.5f,  0.5f,  0.5f };
		vertex.texCoords = { 1,0 };
		vertex.normal = { vertex.position };
		vertices.emplace_back(vertex);

		// 4
		vertex.position = { 0.5f, -0.5f, -0.5f };
		vertex.texCoords = { 0,1 };
		vertex.normal = { vertex.position };
		vertices.emplace_back(vertex);

		// 5
		vertex.position = { 0.5f,  0.5f, -0.5f };
		vertex.texCoords = { 0,0 };
		vertex.normal = { vertex.position };
		vertices.emplace_back(vertex);

		// 6
		vertex.position = { -0.5f, -0.5f, -0.5f };
		vertex.texCoords = { 1,1 };
		vertex.normal = { vertex.position };
		vertices.emplace_back(vertex);

		// 7
		vertex.position = { -0.5f,  0.5f, -0.5f };
		vertex.texCoords = { 1,0 };
		vertex.normal = { vertex.position };
		vertices.emplace_back(vertex);

		unsigned int in[] =
		{
			0,1,2,
			2,3,0,

			3,2,4,
			4,5,3,

			5,4,6,
			6,7,5,

			7,6,1,
			1,0,7,

			3,5,7,
			7,0,3,

			1,6,4,
			4,2,1
		};
		indices.insert(indices.begin(), in, in + 12 * 3);

		//texCoords =
		//{
		//	{0.0f, 1.0f},
		//	{0.0f, 0.0f},
		//	{1.0f, 0.0f},
		//	{1.0f, 1.0f},
		//
		//	{1.0f, 1.0f},
		//	{1.0f, 0.0f},
		//	{0.0f, 1.0f},
		//	{0.0f, 0.0f},
		//};
	}
}

namespace RPyramide
{
	void CreatePyramide(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices)
	{
		Vertex vertex;

		// 0
		vertex.position = { -0.5f,  -0.5f,  0.5f };
		vertex.texCoords = { 1,0 };
		vertex.normal = { vertex.position };
		vertices.emplace_back(vertex);

		// 1
		vertex.position = { 0.5f, -0.5f,  0.5f };
		vertex.texCoords = { 0,0 };
		vertex.normal = { vertex.position };
		vertices.emplace_back(vertex);
		// 2
		vertex.position = { -0.5f, -0.5f, -0.5f };
		vertex.texCoords = { 0,0 };
		vertex.normal = { vertex.position };
		vertices.emplace_back(vertex);
		// 3
		vertex.position = { 0.5f,  -0.5f, -0.5f };
		vertex.texCoords = { 1,0 };
		vertex.normal = { vertex.position };
		vertices.emplace_back(vertex);

		// 4
		vertex.position = { 0.0f, 0.5f, 0.0f };
		vertex.texCoords = { 0.5f,1.0 };
		vertex.normal = { vertex.position };
		vertices.emplace_back(vertex);

		unsigned int in[] =
		{
			1,0,2,

			2,3,1,

			0,1,4,

			1,3,4,

			3,2,4,

			2,0,4

		};
		indices.insert(indices.begin(), in, in + 6 * 3);
	}
}

namespace RSphere
{
	void CreateSphere(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices)
	{
		float x, y, z, xy;                              // vertex position
		float nx, ny, nz, lengthInv = 1.0f / 1.0f;    // vertex normal
		float s, t;                                     // vertex texCoord

		float sectorStep = 2 * M_PI / 20;
		float stackStep = M_PI / 20;
		float sectorAngle, stackAngle;

		for (int i = 0; i <= 20; ++i)
		{
			stackAngle = M_PI / 2 - i * stackStep;    // starting from pi/2 to -pi/2
			xy = 1.0f * cosf(stackAngle);             // r * cos(u)
			z = 1.0f * sinf(stackAngle);              // r * sin(u)

			// add (sectorCount+1) vertices per stack
			// the first and last vertices have same position and normal, but different tex coords
			for (int j = 0; j <= 20; ++j)
			{
				Vertex vertex;

				sectorAngle = j * sectorStep;           // starting from 0 to 2pi

				// vertex position (x, y, z)
				x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
				y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)
				vertex.position = { x, y, z };

				// normalized vertex normal (nx, ny, nz)
				nx = x * lengthInv;
				ny = y * lengthInv;
				nz = z * lengthInv;
				vertex.normal = { nx, ny, nz };

				// vertex tex coord (s, t) range between [0, 1]
				s = (float)j / 20;
				t = (float)i / 20;
				vertex.texCoords = { s, t };

				vertices.push_back(vertex);
			}
		}

		int k1, k2;
		for (int i = 0; i < 20; ++i)
		{
			k1 = i * (20 + 1);     // beginning of current stack
			k2 = k1 + 20 + 1;      // beginning of next stack

			for (int j = 0; j < 20; ++j, ++k1, ++k2)
			{
				// 2 triangles per sector excluding first and last stacks
				// k1 => k2 => k1+1
				if (i != 0)
				{
					indices.push_back(k1);
					indices.push_back(k2);
					indices.push_back(k1 + 1);
				}

				// k1+1 => k2 => k2+1
				if (i != (20 - 1))
				{
					indices.push_back(k1 + 1);
					indices.push_back(k2);
					indices.push_back(k2 + 1);
				}

				// store indices for lines
				// vertical lines for all stacks, k1 => k2
				//lineIndices.push_back(k1);
				//lineIndices.push_back(k2);
				//if (i != 0)  // horizontal lines except 1st stack, k1 => k+1
				//{
				//	lineIndices.push_back(k1);
				//	lineIndices.push_back(k1 + 1);
				//}
			}
		}
	}
}

namespace RCylinder
{
	void CreateCylinder(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices)
	{
		const float PI = 3.1415926f;
		float sectorStep = 2 * PI / 50;
		float sectorAngle;  // radian

		std::vector<float3> unitVertices;
		for (int i = 0; i <= 50; ++i)
		{
			sectorAngle = i * sectorStep;
			unitVertices.emplace_back(cos(sectorAngle), sin(sectorAngle), 0); // x
		}

		//vertices.reserve(204);
		//indices.reserve(600);
		//texCoords.reserve(204);
		//normals.reserve(204);

		for (int i = 0; i < 2; ++i)
		{
			float h = -2.0f / 2.0f + i * 2.0f;           // z value; -h/2 to h/2
			float t = 1.0f - i;                          // vertical tex coord; 1 to 0

			for (int j = 0, k = 0; j <= 50; ++j, ++k)
			{
				Vertex vertex;
				float ux = unitVertices[k].x;
				float uy = unitVertices[k].y;
				float uz = unitVertices[k].z;
				// position vector
				vertex.position = { ux * 1.0f, uy * 1.0f, h };

				// normal vector
				vertex.normal = { ux, uy, uz };

				// texture coordinate
				vertex.texCoords = { (float)j / 50, t };

				vertices.emplace_back(vertex);
			}
		}

		int baseCenterIndex = (int)vertices.size();
		int topCenterIndex = baseCenterIndex + 50 + 1; // include center vertex

		// put base and top vertices to arrays
		for (int i = 0; i < 2; ++i)
		{
			float h = -2.0f / 2.0f + i * 2.0f;           // z value; -h/2 to h/2
			float nz = -1 + i * 2;                           // z value of normal; -1 to 1

			// center point
			Vertex v;
			v.position = { 0,0,h };
			v.normal = { 0,0,nz };
			v.texCoords = { 0.5f,0.5f };
			vertices.emplace_back(v);
			//vertices.emplace_back(0, 0, h);
			//normals.emplace_back(0, 0, nz);
			//texCoords.emplace_back(0.5f, 0.5f);

			for (int j = 0, k = 0; j < 50; ++j, ++k)
			{
				Vertex vertex;
				float ux = unitVertices[k].x;
				float uy = unitVertices[k].y;
				// position vector
				vertex.position = { ux * 1.0f, uy * 1.0f, h };
				
				// normal vector
				vertex.normal = { 0, 0, nz };

				// texture coordinate
				vertex.texCoords = { -ux * 0.5f + 0.5f, -uy * 0.5f + 0.5f };

				vertices.emplace_back(vertex);
			}
		}

		int k1 = 0;                // 1st vertex index at base
		int k2 = 50 + 1;           // 1st vertex index at top

		// indices for the side surface
		for (int i = 0; i < 50; ++i, ++k1, ++k2)
		{
			// 2 triangles per sector
			// k1 => k1+1 => k2
			indices.emplace_back(k1);
			indices.emplace_back(k1 + 1);
			indices.emplace_back(k2);

			// k2 => k1+1 => k2+1
			indices.emplace_back(k2);
			indices.emplace_back(k1 + 1);
			indices.emplace_back(k2 + 1);
		}

		//// indices for the base surface
		////NOTE: baseCenterIndex and topCenterIndices are pre-computed during vertex generation
		////      please see the previous code snippet
		for (int i = 0, k = baseCenterIndex + 1; i < 50; ++i, ++k)
		{
			if (i < 50 - 1)
			{
				indices.emplace_back(baseCenterIndex);
				indices.emplace_back(k + 1);
				indices.emplace_back(k);
			}
			else // last triangle
			{
				indices.emplace_back(baseCenterIndex);
				indices.emplace_back(baseCenterIndex + 1);
				indices.emplace_back(k);
			}
		}

		// indices for the top surface
		for (int i = 0, k = topCenterIndex + 1; i < 50; ++i, ++k)
		{
			if (i < 50 - 1)
			{
				indices.emplace_back(topCenterIndex);
				indices.emplace_back(k);
				indices.emplace_back(k + 1);
			}
			else // last triangle
			{
				indices.emplace_back(topCenterIndex);
				indices.emplace_back(k);
				indices.emplace_back(topCenterIndex + 1);
			}
		}
	}
}

namespace Checker
{
	struct CheckerImage
	{
		int id;
		int width;
		int height;
		GLubyte* checkerImage;
	};

	CheckerImage CreateChecker()
	{
		CheckerImage checker = CheckerImage();
		int size = 64 * 64 * 4;
		checker.checkerImage = new GLubyte[size];
		for (int i = 0; i < size; i += 4)
		{
			GLubyte c = ((i & 0x8) == 0) * 255;
			checker.checkerImage[i] = (GLubyte)c;
			checker.checkerImage[i + 1] = (GLubyte)c;
			checker.checkerImage[i + 2] = (GLubyte)c;
			checker.checkerImage[i + 3] = (GLubyte)255;
		}

		checker.width = 64;
		checker.height = 64;
		checker.id = 0;

		return checker;
	}
}