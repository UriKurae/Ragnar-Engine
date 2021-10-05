#include "Primitive.h"

#include "glew/include/GL/glew.h"

PCube::PCube(float3 t, float3 r, float3 s)
{
	transform = t;
	rotate = r;
	scale = s;

	/*indexVertex.emplace_back(transform.x + scale.x);
	indexVertex.emplace_back(transform.y + scale.y);
	indexVertex.emplace_back(transform.z);

	indexVertex.emplace_back(transform.x);
	indexVertex.emplace_back(transform.y + scale.y);
	indexVertex.emplace_back(transform.z);

	indexVertex.emplace_back(transform.x);
	indexVertex.emplace_back(transform.y);
	indexVertex.emplace_back(transform.z);

	indexVertex.emplace_back(transform.x + scale.x);
	indexVertex.emplace_back(transform.y);
	indexVertex.emplace_back(transform.z);

	indexVertex.emplace_back(transform.x + scale.x);
	indexVertex.emplace_back(transform.y);
	indexVertex.emplace_back(-(transform.z + scale.z));

	indexVertex.emplace_back(transform.x + scale.x);
	indexVertex.emplace_back(transform.y + scale.y);
	indexVertex.emplace_back(-(transform.z + scale.z));

	indexVertex.emplace_back(transform.x + scale.x);
	indexVertex.emplace_back(transform.y + scale.y);
	indexVertex.emplace_back(-(transform.z + scale.z));

	indexVertex.emplace_back(transform.x);
	indexVertex.emplace_back(transform.y + scale.y);
	indexVertex.emplace_back(-(transform.z + scale.z));

	indexVertex.emplace_back(transform.x);
	indexVertex.emplace_back(transform.y);
	indexVertex.emplace_back(-(transform.z + scale.z));


	indices.emplace_back(0);
	indices.emplace_back(1);
	indices.emplace_back(2);
	indices.emplace_back(2);
	indices.emplace_back(3);
	indices.emplace_back(0);
	indices.emplace_back(0);
	indices.emplace_back(3);
	indices.emplace_back(4);
	indices.emplace_back(4);
	indices.emplace_back(5);
	indices.emplace_back(0);
	indices.emplace_back(0);
	indices.emplace_back(5);
	indices.emplace_back(6);
	indices.emplace_back(6);
	indices.emplace_back(1);
	indices.emplace_back(0);
	indices.emplace_back(7);
	indices.emplace_back(6);
	indices.emplace_back(5);
	indices.emplace_back(5);
	indices.emplace_back(4);
	indices.emplace_back(7);
	indices.emplace_back(7);
	indices.emplace_back(2);
	indices.emplace_back(1);
	indices.emplace_back(1);
	indices.emplace_back(6);
	indices.emplace_back(7);
	indices.emplace_back(7);
	indices.emplace_back(4);
	indices.emplace_back(3);
	indices.emplace_back(3);
	indices.emplace_back(2);
	indices.emplace_back(7);*/

	indexBuffer = 0;
	glGenBuffers(1, &indexBuffer);
}

PCube::~PCube()
{
    glDeleteBuffers(1, &indexBuffer);
}

void PCube::Draw()
{
	GLfloat indexVertex[24] =
	{
		1.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f,-1.0f,
		1.0f, 1.0f,-1.0f,
		0.0f, 1.0f,-1.0f,
		0.0f, 0.0f,-1.0f
	};

	GLuint indices[36] =
	{
		0,1,2,
		2,3,0,

		0,3,4,
		4,5,0,

		0,5,6,
		6,1,0,

		7,6,5,
		5,4,7,

		7,2,1,
		1,6,7,

		7,4,3,
		3,2,7
	};

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * 36, &indices, GL_STATIC_DRAW);
	glVertexPointer(3, GL_FLOAT, 0, &indexVertex);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glEnableClientState(GL_VERTEX_ARRAY);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, NULL);
	glDisableClientState(GL_VERTEX_ARRAY);
}

PPlane::PPlane(float3 t, float3 r, float3 s)
{
	transform = t;
	rotate = r;
	scale = s;
}

PPlane::~PPlane()
{
    glDeleteBuffers(1, &indexBuffer);
}

void PPlane::Draw()
{

}

PCylinder::PCylinder(int sectCount, float h, float rad) : sectorCount(sectCount), height(h), radius(rad)
{
    indexBuffer = 0;
    glGenBuffers(1, &indexBuffer);

	glBindBuffer(GL_VERTEX_ARRAY, indexBuffer);
	BuildVerticesSmooth();
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), &indices, GL_STATIC_DRAW);
}

PCylinder::~PCylinder()
{
    glDeleteBuffers(1, &indexBuffer);
}

std::vector<float> PCylinder::GetUnitCircleVertices()
{
    const float PI = 3.1415926f;
    float sectorStep = 2 * PI / sectorCount;
    float sectorAngle;  // radian

    std::vector<float> unitCircleVertices;
    for (int i = 0; i <= sectorCount; ++i)
    {
        sectorAngle = i * sectorStep;
        unitCircleVertices.push_back(cos(sectorAngle)); // x
        unitCircleVertices.push_back(sin(sectorAngle)); // y
        unitCircleVertices.push_back(0);                // z
    }

    return unitCircleVertices;
}

void PCylinder::Draw()
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glBindBuffer(GL_VERTEX_ARRAY, indexBuffer);
	glVertexPointer(3, GL_FLOAT, 0, &vertices[0]);
	glNormalPointer(GL_FLOAT, 0, &normals[0]);
	glTexCoordPointer(2, GL_FLOAT, 0, &texCoords[0]);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, &indices[0]);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

void PCylinder::BuildVerticesSmooth()
{
    // clear memory of prev arrays
    std::vector<GLfloat>().swap(vertices);
    std::vector<GLfloat>().swap(normals);
    std::vector<GLfloat>().swap(texCoords);

    // get unit circle vectors on XY-plane
    std::vector<float> unitVertices = GetUnitCircleVertices();

    // put side vertices to arrays
    for (int i = 0; i < 2; ++i)
    {
        float h = -height / 2.0f + i * height;           // z value; -h/2 to h/2
        float t = 1.0f - i;                              // vertical tex coord; 1 to 0

        for (int j = 0, k = 0; j <= sectorCount; ++j, k += 3)
        {
            float ux = unitVertices[k];
            float uy = unitVertices[k + 1];
            float uz = unitVertices[k + 2];
            // position vector
            vertices.push_back(ux * radius);             // vx
            vertices.push_back(uy * radius);             // vy
            vertices.push_back(h);                       // vz
            // normal vector
            normals.push_back(ux);                       // nx
            normals.push_back(uy);                       // ny
            normals.push_back(uz);                       // nz
            // texture coordinate
            texCoords.push_back((float)j / sectorCount); // s
            texCoords.push_back(t);                      // t
        }
    }

    // the starting index for the base/top surface
    //NOTE: it is used for generating indices later
    int baseCenterIndex = (int)vertices.size() / 3;
    int topCenterIndex = baseCenterIndex + sectorCount + 1; // include center vertex

    // put base and top vertices to arrays
    for (int i = 0; i < 2; ++i)
    {
        float h = -height / 2.0f + i * height;           // z value; -h/2 to h/2
        float nz = -1 + i * 2;                           // z value of normal; -1 to 1

        // center point
        vertices.push_back(0);     vertices.push_back(0);     vertices.push_back(h);
        normals.push_back(0);      normals.push_back(0);      normals.push_back(nz);
        texCoords.push_back(0.5f); texCoords.push_back(0.5f);

        for (int j = 0, k = 0; j < sectorCount; ++j, k += 3)
        {
            float ux = unitVertices[k];
            float uy = unitVertices[k + 1];
            // position vector
            vertices.push_back(ux * radius);             // vx
            vertices.push_back(uy * radius);             // vy
            vertices.push_back(h);                       // vz
            // normal vector
            normals.push_back(0);                        // nx
            normals.push_back(0);                        // ny
            normals.push_back(nz);                       // nz
            // texture coordinate
            texCoords.push_back(-ux * 0.5f + 0.5f);      // s
            texCoords.push_back(-uy * 0.5f + 0.5f);      // t
        }
    }

    // generate CCW index list of cylinder triangles
    int k1 = 0;                         // 1st vertex index at base
    int k2 = sectorCount + 1;           // 1st vertex index at top

    // indices for the side surface
    for (int i = 0; i < sectorCount; ++i, ++k1, ++k2)
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
    for (int i = 0, k = baseCenterIndex + 1; i < sectorCount; ++i, ++k)
    {
        if (i < sectorCount - 1)
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
    for (int i = 0, k = topCenterIndex + 1; i < sectorCount; ++i, ++k)
    {
        if (i < sectorCount - 1)
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

PPyramid::PPyramid(float3 t, float3 r, float3 s)
{
	transform = t;
	rotate = r;
	scale = s;
    glGenBuffers(1, &indexBuffer);
}

PPyramid::~PPyramid()
{
    glDeleteBuffers(1, &indexBuffer);
}

void PPyramid::Draw()
{
	float vertices[15] =
	{
		transform.x, transform.y, transform.z,  //0
		transform.x + scale.x, transform.y, transform.z,  //1
		transform.x, transform.y, -(transform.z + scale.z), //2
		transform.x + scale.x, transform.y, -(transform.z + scale.z),  //3
		transform.x + (scale.x / 2), transform.y + scale.y, -(transform.z + (scale.z / 2))  //4
	};

	int indices[18] =
	{
		1,0,2,

		2,3,1,

		0,1,4,

		1,3,4,

		3,2,4,

		2,0,4
	};

	/*int indices[18] =
	{
		0,2,3,

		3,1,2,

		0,1,4,

		1,3,4,

		3,2,4,

		2,0,4
	};*/

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * 18, indices, GL_STATIC_DRAW);
	glVertexPointer(3, GL_FLOAT, 0, &vertices);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glEnableClientState(GL_VERTEX_ARRAY);
	glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, NULL);
	glDisableClientState(GL_VERTEX_ARRAY);
}

PSphere::PSphere()
{
	indexBuffer = 0;
	glGenBuffers(1, &indexBuffer);
}

PSphere::~PSphere()
{
	glDeleteBuffers(1, &indexBuffer);
}

void PSphere::Draw()
{
}