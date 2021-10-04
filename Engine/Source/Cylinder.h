#pragma once

#include "glew/include/GL/glew.h"
#include <vector>

class Cylinder
{
public:

	Cylinder(int sectCount, float h, float rad);
	~Cylinder();

	std::vector<float> GetUnitCircleVertices();
	

	void buildVerticesSmooth();
	void buildIndices();

public:

	int sectorCount;
	float height;
	float radius;

	std::vector<GLfloat> vertices;
	std::vector<GLfloat> normals;
	std::vector<GLfloat> texCoords;
	std::vector<GLuint> indices;
};