#pragma once

#include <vector>
#include "MathGeoLib/src/MathGeoLib.h"

typedef unsigned char GLubyte;

namespace RCube
{
	void CreateCube(std::vector<float3>& vertices, std::vector<unsigned int>& indices, std::vector<float2>& texCoords)
	{
		vertices =
		{
			{-0.5f, 0.5f, 0.5f},
			{-0.5f, -0.5f, 0.5f},
			{0.5f, -0.5f, 0.5f},
			{0.5f, 0.5f, 0.5f},

			{0.5f, -0.5f, -0.5f},
			{0.5f, 0.5f, -0.5f},
			{-0.5f, -0.5f, -0.5f},
			{-0.5f, 0.5f, -0.5f},
		};

		indices =
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

		texCoords =
		{
			{0.0f, 1.0f},
			{0.0f, 0.0f},
			{1.0f, 0.0f},
			{1.0f, 1.0f},

			{0.0f, 1.0f},
			{0.0f, 0.0f},
			{1.0f, 0.0f},
			{1.0f, 1.0f},
		};
	}
}

namespace RPyramide
{
	void CreatePyramide(std::vector<float3>& vertices, std::vector<unsigned int>& indices, std::vector<float2>& texCoords)
	{
		vertices =
		{
			{-0.5f, -0.5f, 0.5f},
			{0.5f, -0.5f, 0.5f},
			{-0.5f, -0.5f, -0.5f},
			{0.5f, -0.5f, -0.5f},
			{0.0f, 0.5f, 0.0f},
		};

		indices =
		{
			1,0,2,

			2,3,1,

			0,1,4,

			1,3,4,

			3,2,4,

			2,0,4
		};

		texCoords =
		{
			{0.0f, 0.0f},
			{0.0f, 0.0f},
			{0.0f, 0.0f},
			{0.0f, 0.0f},
			{0.0f, 0.0f},
		};
	}
}

namespace RSphere
{
	void CreateSphere(std::vector<float3>& vertices, std::vector<unsigned int>& indices, std::vector<float2>& texCoords)
	{
		float const R = 1. / (float)(20 - 1);
		float const S = 1. / (float)(20 - 1);
		unsigned int r, s;
		int radius = 1;

		vertices.reserve(20 * 20 * 3);
		indices.reserve(20 * 20 * 4);
		texCoords.reserve(20 * 20 * 3);

		for (r = 0; r < 20; ++r)
		{
			for (s = 0; s < 20; ++s)
			{
				float const y = sin(-M_PI_2 + M_PI * r * R);
				float const x = cos(2 * M_PI * s * S) * sin(M_PI * r * R);
				float const z = sin(2 * M_PI * s * S) * sin(M_PI * r * R);

				vertices.emplace_back(x * radius);
				vertices.emplace_back(y * radius);
				vertices.emplace_back(z * radius);

				indices.emplace_back(r * 20 + s);
				indices.emplace_back(r * 20 + (s + 1));
				indices.emplace_back((r + 1) * 20 + (s + 1));
				indices.emplace_back((r + 1) * 20 + s);

				texCoords.emplace_back(x * radius);
				texCoords.emplace_back(y * radius);
				texCoords.emplace_back(z * radius);
			}
		}
	}
}

namespace RCylinder
{
	void CreateCylinder(std::vector<float3>& vertices, std::vector<unsigned int>& indices, std::vector<float2>& texCoords)
	{
		// get unit circle vectors on XY-plane
		std::vector<float> unitVertices;

		const float PI = 3.1415926f;
		float sectorStep = 2 * PI / 50;
		float sectorAngle;  // radian

		for (int i = 0; i <= 50; ++i)
		{
			sectorAngle = i * sectorStep;
			unitVertices.push_back(cos(sectorAngle)); // x
			unitVertices.push_back(sin(sectorAngle)); // y
			unitVertices.push_back(0);                // z
		}

		// put side vertices to arrays
		for (int i = 0; i < 2; ++i)
		{
			float h = -2.0f / 2.0f + i * 2.0f;           // z value; -h/2 to h/2
			float t = 1.0f - i;                              // vertical tex coord; 1 to 0

			for (int j = 0, k = 0; j <= 50; ++j, k += 3)
			{
				float ux = unitVertices[k];
				float uy = unitVertices[k + 1];
				float uz = unitVertices[k + 2];
				// position vector
				vertices.push_back(float3(ux * 1.0f, uy * 1.0f, h));
                   
				// normal vector
				//normals.push_back(float3(ux, uy, uz));                    
				
				// texture coordinate
				texCoords.push_back(float2((float)j / 50, t));
			}
		}

		// the starting index for the base/top surface
		//NOTE: it is used for generating indices later
		int baseCenterIndex = (int)vertices.size() / 3;
		int topCenterIndex = baseCenterIndex + 50 + 1; // include center vertex

		// put base and top vertices to arrays
		for (int i = 0; i < 2; ++i)
		{
			float h = -2.0f / 2.0f + i * 2.0f;           // z value; -h/2 to h/2
			float nz = -1.0f + i * 2.0f;                           // z value of normal; -1 to 1

			// center point
			vertices.push_back(float3(0.0f, 0.0f, h));
			//normals.push_back(float3(0, 0, nz));
			texCoords.push_back(float2(0.5f, 0.5f));

			for (int j = 0, k = 0; j < 50; ++j, k += 3)
			{
				float ux = unitVertices[k];
				float uy = unitVertices[k + 1];
				// position vector
				vertices.push_back(float3(ux * 1.0f, uy * 1.0f, h));
				
				// normal vector
				//normals.push_back(float3(0.0f, 0.0f, nz));

				// texture coordinate
				texCoords.push_back(float2(-ux * 0.5f + 0.5f, -uy * 0.5f + 0.5f));
			}
		}

		// generate CCW index list of cylinder triangles
		int k1 = 0;                         // 1st vertex index at base
		int k2 = 50 + 1;           // 1st vertex index at top

		// indices for the side surface
		for (int i = 0; i < 50; ++i, ++k1, ++k2)
		{
			// 2 triangles per sector
			// k1 => k1+1 => k2
			indices.push_back(k1);
			indices.push_back(k1 + 1);
			indices.push_back(k2);

			// k2 => k1+1 => k2+1
			indices.push_back(k2);
			indices.push_back(k1 + 1);
			indices.push_back(k2 + 1);
		}

		// indices for the base surface
		//NOTE: baseCenterIndex and topCenterIndices are pre-computed during vertex generation
		//      please see the previous code snippet
		for (int i = 0, k = baseCenterIndex + 1; i < 50; ++i, ++k)
		{
			if (i < 50 - 1)
			{
				indices.push_back(baseCenterIndex);
				indices.push_back(k + 1);
				indices.push_back(k);
			}
			else // last triangle
			{
				indices.push_back(baseCenterIndex);
				indices.push_back(baseCenterIndex + 1);
				indices.push_back(k);
			}
		}

		// indices for the top surface
		for (int i = 0, k = topCenterIndex + 1; i < 50; ++i, ++k)
		{
			if (i < 50 - 1)
			{
				indices.push_back(topCenterIndex);
				indices.push_back(k);
				indices.push_back(k + 1);
			}
			else // last triangle
			{
				indices.push_back(topCenterIndex);
				indices.push_back(k);
				indices.push_back(topCenterIndex + 1);
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