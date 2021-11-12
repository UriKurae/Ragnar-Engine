#include "Primitive.h"

#include "glew/include/GL/glew.h"
#include "Globals.h"

#include "Profiling.h"

//PCube::PCube(float3 t, float3 r, float3 s) : Primitive()
//{
//	transform = t;
//	rotate = r;
//	scale = s;
//	/*
//	indexVertex.push_back(transform.x + scale.x);
//	indexVertex.push_back(transform.y + scale.y);
//	indexVertex.push_back(transform.z);
//	
//	indexVertex.push_back(transform.x);
//	indexVertex.push_back(transform.y + scale.y);
//	indexVertex.push_back(transform.z);
//	
//	indexVertex.push_back(transform.x);
//	indexVertex.push_back(transform.y);
//	indexVertex.push_back(transform.z);
//	
//	indexVertex.push_back(transform.x + scale.x);
//	indexVertex.push_back(transform.y);
//	indexVertex.push_back(transform.z);
//		
//	indexVertex.push_back(transform.x + scale.x);
//	indexVertex.push_back(transform.y);
//	indexVertex.push_back((transform.z + scale.z));
//		
//	indexVertex.push_back(transform.x + scale.x);
//	indexVertex.push_back(transform.y + scale.y);
//	indexVertex.push_back((transform.z + scale.z));
//			
//	indexVertex.push_back(transform.x);
//	indexVertex.push_back(transform.y + scale.y);
//	indexVertex.push_back((transform.z + scale.z));
//				
//	indexVertex.push_back(transform.x);
//	indexVertex.push_back(transform.y);
//	indexVertex.push_back((transform.z + scale.z));
//	*/
//
//	/*GLfloat indexVertex[24] =
//	{
//		1.0f, 1.0f, 0.0f,
//		0.0f, 1.0f, 0.0f,
//		0.0f, 0.0f, 0.0f,
//		1.0f, 0.0f, 0.0f,
//		1.0f, 0.0f, 1.0f,
//		1.0f, 1.0f, 1.0f,
//		0.0f, 1.0f, 1.0f,
//		0.0f, 0.0f, 1.0f
//	};*/
//	/*
//	GLuint indices[36] =
//	{
//		0,1,2,
//		2,3,0,
//
//		0,3,4,
//		4,5,0,
//
//		0,5,6,
//		6,1,0,
//
//		7,6,5,
//		5,4,7,
//
//		7,2,1,
//		1,6,7,
//
//		7,4,3,
//		3,2,7
//	};
//	*/
//
//	/*
//	texCoords.push_back(float2(1.f, 1.f));
//	texCoords.push_back(float2(0.f, 1.f));
//	texCoords.push_back(float2(0.f, 0.f));
//
//	texCoords.push_back(float2(0.f, 0.f));
//	texCoords.push_back(float2(1.f, 0.f));
//	texCoords.push_back(float2(1.f, 1.f));
//	*/
//
//	//////////////////////////////////// TEST
//	/*
//		Aqui teniais un buen follon. Teniais 24 vertices y solo 6 coordenadas de texturas. Tiene que haver una coordenada por vertice.
//		De todas maneras, usando indices solo necesitais 8 vertices y reusarlos en todas las caras.
//		Es decir:
//		Un cubo tiene:
//
//			8 vertices(float3)
//			8 coords de texture(float2)
//			36 indices
//
//		Os he hecho una cara para comprobar que la textura se aplica correctamente, extendedlo vosotros.
//
//		Si quereis hacerlo parametrico pasadle la longitud por parametro y en lugar de 0.5 usad longitud/2
//	*/
//	// Frontal vertices
//	indexVertex.push_back(-0.5f);
//	indexVertex.push_back(0.5f);
//	indexVertex.push_back(0.0f);
//
//	indexVertex.push_back(-0.5f);
//	indexVertex.push_back(-0.5f);
//	indexVertex.push_back(0.0f);
//
//	indexVertex.push_back(0.5f);
//	indexVertex.push_back(-0.5f);
//	indexVertex.push_back(0.f);
//
//	indexVertex.push_back(0.5f);
//	indexVertex.push_back(0.5f);
//	indexVertex.push_back(0.0f);
//
//	// Back vertices
//	indexVertex.push_back(0.5f);
//	indexVertex.push_back(-0.5f);
//	indexVertex.push_back(-1.0f);
//
//	indexVertex.push_back(0.5f);
//	indexVertex.push_back(0.5f);
//	indexVertex.push_back(-1.0f);
//
//	indexVertex.push_back(-0.5f);
//	indexVertex.push_back(-0.5f);
//	indexVertex.push_back(-1.0f);
//
//	indexVertex.push_back(-0.5f);
//	indexVertex.push_back(0.5f);
//	indexVertex.push_back(-1.0f);
//
//	texCoords.push_back(float2(0.0f, 1.0f)); // 0 
//	texCoords.push_back(float2(0.0f, 0.0f)); // 1
//	texCoords.push_back(float2(1.0f, 0.0f)); // 2
//	texCoords.push_back(float2(1.0f, 1.0f)); // 3
//
//	texCoords.push_back(float2(0.0f, 1.0f)); // 5
//	texCoords.push_back(float2(0.0f, 0.0f)); // 4
//	texCoords.push_back(float2(1.0f, 0.0f)); // 6
//	texCoords.push_back(float2(1.0f, 1.0f)); // 7
//
//	GLuint indices[36] =
//	{
//		0,1,2,
//		2,3,0,
//
//		3,2,4,
//		4,5,3,
//
//		5,4,6,
//		6,7,5,
//
//		7,6,1,
//		1,0,7,
//
//		3,5,7,
//		7,0,3, 
//
//		1,6,4,
//		4,2,1
//
//	};
//
//	//////////////////////////////////// 
//	CreateCheckerImage();
//
//	//glGenVertexArrays(1, &vao);
//	//glBindVertexArray(vao);
//
//	// Generate all buffers and setup them
//	glGenBuffers(1, &tbo);
//	glGenTextures(1, &texId);
//
//	vertex = new VertexBuffer(indexVertex.data(), indexVertex.size() * sizeof(GLfloat));
//	index = new IndexBuffer(indices, 36);
//	
//	glBindBuffer(GL_ARRAY_BUFFER, tbo);
//	glBufferData(GL_ARRAY_BUFFER, texCoords.size() * sizeof(float2), texCoords.data(), GL_STATIC_DRAW);
//
//	glBindTexture(GL_TEXTURE_2D, texId);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 128, 128, 0, GL_RGBA, GL_UNSIGNED_BYTE, checkerImage);
//
//
//	// Unbind all buffers
//	glBindTexture(GL_TEXTURE_2D, 0);
//	index->Unbind();
//	vertex->Unbind();
//	glBindBuffer(GL_ARRAY_BUFFER, 0);
//
//	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 3, 0);
//	//glEnableVertexAttribArray(0);
//}
//
//PCube::~PCube()
//{
//	RELEASE(vertex);
//	RELEASE(index);
//}
//
//void PCube::Draw()
//{
//	glEnableClientState(GL_VERTEX_ARRAY);
//	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
//	//glBindVertexArray(vao);
//
//	vertex->Bind();
//	glVertexPointer(3, GL_FLOAT, 0, NULL);
//
//	glBindBuffer(GL_ARRAY_BUFFER, tbo);
//	glTexCoordPointer(2, GL_FLOAT, 0, NULL);
//
//	glBindTexture(GL_TEXTURE_2D, texId);
//
//	index->Bind();
//
//	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, NULL);
//
//	index->Unbind();
//	vertex->Unbind();
//	glBindBuffer(GL_ARRAY_BUFFER, 0);
//	glBindTexture(GL_TEXTURE_2D, 0);
//
//	glDisableClientState(GL_VERTEX_ARRAY);
//	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
//
//	//glBindVertexArray(vao);
//	//glDrawElements(GL_TRIANGLES, index->GetSize(), GL_UNSIGNED_INT, NULL);
//	//glBindVertexArray(0);
//}
//
//void PCube::CreateCheckerImage()
//{
//	for (int i = 0; i < 128; ++i)
//	{
//		for (int j = 0; j < 128; ++j)
//		{
//			GLubyte c = ((((i & 0x8) == 0) ^ (((j & 0x8)) == 0))) * 255;
//			checkerImage[i][j][0] = (GLubyte)c;
//			checkerImage[i][j][1] = (GLubyte)c;
//			checkerImage[i][j][2] = (GLubyte)c;
//			checkerImage[i][j][3] = (GLubyte)255;
//		}
//	}
//}
//
//PPlane::PPlane(float3 t, float3 r, float3 s) : Primitive()
//{
//	transform = t;
//	rotate = r;
//	scale = s;
//}
//
//PPlane::~PPlane()
//{
//    /*glDeleteBuffers(1, &indexBuffer);*/
//}
//
//void PPlane::Draw()
//{
//
//}
//
//PCylinder::PCylinder(int sectCount, float h, float rad) : sectorCount(sectCount), height(h), radius(rad)
//{
//	BuildVerticesSmooth();
//	
//	glGenVertexArrays(1, &vao);
//	glBindVertexArray(vao);
//
//	vertex = new VertexBuffer(vertices.data(), vertices.size() * sizeof(GLfloat));
//	index = new IndexBuffer(indices.data(), indices.size());
//
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 3, 0);
//	glEnableVertexAttribArray(0);
//
//	//glVertexPointer(3, GL_FLOAT, 0, vertices.data());
//	//glNormalPointer(GL_FLOAT, 0, normals.data());
//	//glTexCoordPointer(2, GL_FLOAT, 0, texCoords.data());
//}
//
//PCylinder::~PCylinder()
//{
///*   glDeleteBuffers(1, &indexBuffer);*/
//}
//
//std::vector<float> PCylinder::GetUnitCircleVertices()
//{
//    const float PI = 3.1415926f;
//    float sectorStep = 2 * PI / sectorCount;
//    float sectorAngle;  // radian
//
//    std::vector<float> unitCircleVertices;
//    for (int i = 0; i <= sectorCount; ++i)
//    {
//        sectorAngle = i * sectorStep;
//        unitCircleVertices.push_back(cos(sectorAngle)); // x
//        unitCircleVertices.push_back(sin(sectorAngle)); // y
//        unitCircleVertices.push_back(0);                // z
//    }
//
//    return unitCircleVertices;
//}
//
//void PCylinder::Draw()
//{
//	glBindVertexArray(vao);
//	glDrawElements(GL_TRIANGLES, index->GetSize(), GL_UNSIGNED_INT, NULL);
//	glBindVertexArray(0);
//}
//
//void PCylinder::BuildVerticesSmooth()
//{
//    // clear memory of prev arrays
//    std::vector<GLfloat>().swap(vertices);
//    std::vector<GLfloat>().swap(normals);
//    std::vector<GLfloat>().swap(texCoords);
//
//    // get unit circle vectors on XY-plane
//    std::vector<float> unitVertices = GetUnitCircleVertices();
//
//    // put side vertices to arrays
//    for (int i = 0; i < 2; ++i)
//    {
//        float h = -height / 2.0f + i * height;           // z value; -h/2 to h/2
//        float t = 1.0f - i;                              // vertical tex coord; 1 to 0
//
//        for (int j = 0, k = 0; j <= sectorCount; ++j, k += 3)
//        {
//            float ux = unitVertices[k];
//            float uy = unitVertices[k + 1];
//            float uz = unitVertices[k + 2];
//            // position vector
//            vertices.push_back(ux * radius);             // vx
//            vertices.push_back(uy * radius);             // vy
//            vertices.push_back(h);                       // vz
//            // normal vector
//            normals.push_back(ux);                       // nx
//            normals.push_back(uy);                       // ny
//            normals.push_back(uz);                       // nz
//            // texture coordinate
//            texCoords.push_back((float)j / sectorCount); // s
//            texCoords.push_back(t);                      // t
//        }
//    }
//
//    // the starting index for the base/top surface
//    //NOTE: it is used for generating indices later
//    int baseCenterIndex = (int)vertices.size() / 3;
//    int topCenterIndex = baseCenterIndex + sectorCount + 1; // include center vertex
//
//    // put base and top vertices to arrays
//    for (int i = 0; i < 2; ++i)
//    {
//        float h = -height / 2.0f + i * height;           // z value; -h/2 to h/2
//        float nz = -1.0f + i * 2.0f;                           // z value of normal; -1 to 1
//
//        // center point
//        vertices.push_back(0);     vertices.push_back(0);     vertices.push_back(h);
//        normals.push_back(0);      normals.push_back(0);      normals.push_back(nz);
//        texCoords.push_back(0.5f); texCoords.push_back(0.5f);
//
//        for (int j = 0, k = 0; j < sectorCount; ++j, k += 3)
//        {
//            float ux = unitVertices[k];
//            float uy = unitVertices[k + 1];
//            // position vector
//            vertices.push_back(ux * radius);             // vx
//            vertices.push_back(uy * radius);             // vy
//            vertices.push_back(h);                       // vz
//            // normal vector
//            normals.push_back(0);                        // nx
//            normals.push_back(0);                        // ny
//            normals.push_back(nz);                       // nz
//            // texture coordinate
//            texCoords.push_back(-ux * 0.5f + 0.5f);      // s
//            texCoords.push_back(-uy * 0.5f + 0.5f);      // t
//        }
//    }
//
//    // generate CCW index list of cylinder triangles
//    int k1 = 0;                         // 1st vertex index at base
//    int k2 = sectorCount + 1;           // 1st vertex index at top
//
//    // indices for the side surface
//    for (int i = 0; i < sectorCount; ++i, ++k1, ++k2)
//    {
//        // 2 triangles per sector
//        // k1 => k1+1 => k2
//        indices.push_back(k1);
//        indices.push_back(k1 + 1);
//        indices.push_back(k2);
//
//        // k2 => k1+1 => k2+1
//        indices.push_back(k2);
//        indices.push_back(k1 + 1);
//        indices.push_back(k2 + 1);
//    }
//
//    // indices for the base surface
//    //NOTE: baseCenterIndex and topCenterIndices are pre-computed during vertex generation
//    //      please see the previous code snippet
//    for (int i = 0, k = baseCenterIndex + 1; i < sectorCount; ++i, ++k)
//    {
//        if (i < sectorCount - 1)
//        {
//            indices.push_back(baseCenterIndex);
//            indices.push_back(k + 1);
//            indices.push_back(k);
//        }
//        else // last triangle
//        {
//            indices.push_back(baseCenterIndex);
//            indices.push_back(baseCenterIndex + 1);
//            indices.push_back(k);
//        }
//    }
//
//    // indices for the top surface
//    for (int i = 0, k = topCenterIndex + 1; i < sectorCount; ++i, ++k)
//    {
//        if (i < sectorCount - 1)
//        {
//            indices.push_back(topCenterIndex);
//            indices.push_back(k);
//            indices.push_back(k + 1);
//        }
//        else // last triangle
//        {
//            indices.push_back(topCenterIndex);
//            indices.push_back(k);
//            indices.push_back(topCenterIndex + 1);
//        }
//    }
//}
//
//PPyramid::PPyramid(float3 t, float3 r, float3 s) : Primitive()
//{
//	transform = t;
//	rotate = r;
//	scale = s;
//
//	float vertices[15] =
//	{
//		transform.x, transform.y, transform.z,  //0
//		transform.x + scale.x, transform.y, transform.z,  //1
//		transform.x, transform.y, -(transform.z + scale.z), //2
//		transform.x + scale.x, transform.y, -(transform.z + scale.z),  //3
//		transform.x + (scale.x / 2), transform.y + scale.y, -(transform.z + (scale.z / 2))  //4
//	};
//
//	unsigned int indices[18] =
//	{
//		1,0,2,
//
//		2,3,1,
//
//		0,1,4,
//
//		1,3,4,
//
//		3,2,4,
//
//		2,0,4
//	};
//
//	/*vertices.push_back(transform.x);
//	vertices.push_back(transform.y);
//	vertices.push_back(transform.z);
//
//	vertices.push_back(transform.x + scale.x);
//	vertices.push_back(transform.y);
//	vertices.push_back(transform.z);
//
//	vertices.push_back(transform.x);
//	vertices.push_back(transform.y);
//	vertices.push_back(-(transform.z + scale.z));
//	
//	vertices.push_back(transform.x + scale.x);
//	vertices.push_back(transform.y);
//	vertices.push_back(-(transform.z + scale.z));
//
//	vertices.push_back(transform.x + (scale.x / 2));
//	vertices.push_back(transform.y + scale.y);
//	vertices.push_back(-(transform.z + (scale.z / 2)));
//
//
//	indices.push_back(1);
//	indices.push_back(0);
//	indices.push_back(2);
//
//	indices.push_back(2);
//	indices.push_back(3);
//	indices.push_back(1);
//
//	indices.push_back(0);
//	indices.push_back(1);
//	indices.push_back(4);
//
//	indices.push_back(1);
//	indices.push_back(3);
//	indices.push_back(4);
//
//	indices.push_back(3);
//	indices.push_back(2);
//	indices.push_back(4);
//
//	indices.push_back(2);
//	indices.push_back(0);
//	indices.push_back(4);*/
//
//	glGenVertexArrays(1, &vao);
//	glBindVertexArray(vao);
//
//	vertex = new VertexBuffer(vertices, 15 * sizeof(GLfloat));
//	index = new IndexBuffer(indices, 18);
//
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 3, 0);
//	glEnableVertexAttribArray(0);
//}
//
//PPyramid::~PPyramid()
//{
//    //glDeleteBuffers(1, &indexBuffer);
//}
//
//void PPyramid::Draw()
//{
//	/*glEnableClientState(GL_VERTEX_ARRAY);
//	glBindVertexArray(vao);
//	index->Bind();
//	glDrawElements(GL_TRIANGLES, index->GetSize(), GL_UNSIGNED_INT, NULL);
//	index->Unbind();
//	glDisableClientState(GL_VERTEX_ARRAY);*/
//
//	glBindVertexArray(vao);
//	glDrawElements(GL_TRIANGLES, index->GetSize(), GL_UNSIGNED_INT, NULL);
//	glBindVertexArray(0);
//}

//PSphere::PSphere(float radius, unsigned int rings, unsigned int sectors) : Primitive() 
//{
//	float const R = 1. / (float)(rings - 1);
//	float const S = 1. / (float)(sectors - 1);
//	unsigned int r, s;
//
//	vertices.resize(rings * sectors * 3);
//	normals.resize(rings * sectors * 3);
//	texCoords.resize(rings * sectors * 3);
//
//	std::vector<GLfloat>::iterator v = vertices.begin();
//	std::vector<GLfloat>::iterator n = normals.begin();
//	std::vector<GLfloat>::iterator t = texCoords.begin();
//	for (r = 0; r < rings; r++)
//	{
//		for (s = 0; s < sectors; s++)
//		{
//			float const y = sin(-M_PI_2 + M_PI * r * R);
//			float const x = cos(2 * M_PI * s * S) * sin(M_PI * r * R);
//			float const z = sin(2 * M_PI * s * S) * sin(M_PI * r * R);
//
//			*t++ = s * S;
//			*t++ = r * R;
//
//			*v++ = x * radius;
//			*v++ = y * radius;
//			*v++ = z * radius;
//
//			*n++ = x;
//			*n++ = y;
//			*n++ = z;
//		}
//	}
//
//	indices.resize(rings * sectors * 4);
//	std::vector<GLushort>::iterator i = indices.begin();
//	for (r = 0; r < rings; r++)
//	{
//		for (s = 0; s < sectors; s++)
//		{
//			*i++ = r * sectors + s;
//			*i++ = r * sectors + (s + 1);
//			*i++ = (r + 1) * sectors + (s + 1);
//			*i++ = (r + 1) * sectors + s;
//		}
//	}
//
//	glGenVertexArrays(1, &vao);
//	glBindVertexArray(vao);
//
//	vertex = new VertexBuffer(vertices.data(), vertices.size() * sizeof(GLfloat));
//	index = new IndexBuffer(indices.data(), indices.size());
//
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 3, 0);
//	glEnableVertexAttribArray(0);
//
//
//	//indexBuffer = 0;
//	//glGenBuffers(1, &indexBuffer);
//
//	//glBindBuffer(GL_VERTEX_ARRAY, indexBuffer);
//	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * indices.size(), indices.data(), GL_STATIC_DRAW);
//	//glVertexPointer(3, GL_FLOAT, 0, &vertices[0]);
//	//glNormalPointer(GL_FLOAT, 0, &normals[0]);
//	//glTexCoordPointer(2, GL_FLOAT, 0, &texCoords[0]);
//}
//
//PSphere::~PSphere()
//{
//	//glDeleteBuffers(1, &indexBuffer);
//}
//
//void PSphere::Draw()
//{
//	/*glEnableClientState(GL_VERTEX_ARRAY);
//	glEnableClientState(GL_NORMAL_ARRAY);
//	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
//
//	glBindVertexArray(vao);
//	index->Bind();
//	glDrawElements(GL_QUADS, indices.size(), GL_UNSIGNED_SHORT, indices.data());
//	index->Unbind();
//
//	glDisableClientState(GL_VERTEX_ARRAY);
//	glDisableClientState(GL_NORMAL_ARRAY);
//	glDisableClientState(GL_TEXTURE_COORD_ARRAY);*/
//
//	glBindVertexArray(vao);
//	glDrawElements(GL_QUADS, indices.size(), GL_UNSIGNED_SHORT, NULL);
//	glBindVertexArray(0);
//}

PGrid::PGrid(float w, float h) : width(w), height(h), Primitive()
{
	CreateGrid();
	transform = { 0.0f, 0.0f, 0.0f };
	vertex = new VertexBuffer(vertices.data(), sizeof(float3) * vertices.size());
}

PGrid::~PGrid()
{
	RELEASE(vertex);
}

void PGrid::Draw()
{
	glEnableClientState(GL_VERTEX_ARRAY);
	vertex->Bind();
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	glDrawArrays(GL_LINES, 0, vertex->GetSize());

	DrawAxis();

	vertex->Unbind();
	glDisableClientState(GL_VERTEX_ARRAY);

}

void PGrid::CreateGrid()
{
	
	for (int j = 0; j < width; ++j)
	{
		float3 line = {};
		line = { (-width / 2) + (j), 0, (-width / 2)};
		vertices.push_back(line);

		line = { (-width / 2) + (j), 0, (width / 2) };
		vertices.push_back(line);
	}

	for (int i = 0; i < height; ++i)
	{
		float3 line = {};
		line = { (-width / 2), 0, (-width / 2) + (i) };
		vertices.push_back(line);

		line = { (width / 2), 0, (-width / 2) + (i) };
		vertices.push_back(line);
	}	
}

void Primitive::DrawAxis()
{
	glLineWidth(3.0f);

	glBegin(GL_LINES);

	// X axis
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(transform.x, transform.y, transform.z);
	glVertex3f(transform.x + 1.0f, transform.y, transform.z);

	// Y axis
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(transform.x, transform.y, transform.z);
	glVertex3f(transform.x, transform.y + 1.0f, transform.z);

	// X axis
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(transform.x, transform.y, transform.z);
	glVertex3f(transform.x, transform.y, transform.z + 1.0f);

	glColor3f(1.0f, 1.0f, 1.0f);


	glEnd();

	glLineWidth(1.0f);
}