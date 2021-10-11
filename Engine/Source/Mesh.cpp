#include "Mesh.h"

#include "glew/include/GL/glew.h"
#include "MathGeoLib/src/MathGeoLib.h"

Mesh::Mesh(std::vector<Vertex> vert, std::vector<unsigned int> ind, std::vector<Texture> text) : vertices(vert), indices(ind), textures(text), vao(0), vbo(0), ebo(0)
{
	SetupMesh();
}

void Mesh::Draw()
{
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	/*glBegin(GL_LINES);
	for (int i = 0; i < vertices.size(); ++i)
	{
		glVertex3f(vertices[i].position.x, vertices[i].position.y, vertices[i].position.z);
		float3 vertexNorm;
		vertexNorm.x = vertices[i].position.x + vertices[i].normal.x;
		vertexNorm.y = vertices[i].position.y + vertices[i].normal.y;
		vertexNorm.z = vertices[i].position.z + vertices[i].normal.z;
		glVertex3f(vertexNorm.x, vertexNorm.y, vertexNorm.z);
	}
	glEnd();*/

	/*glBegin(GL_LINES);
	for (int i = 0; i < vertices.size(); i+=3)
	{
		float3 line1 = -(vertices[i].position - vertices[i+1].position);
		float3 line2 = (vertices[i+1].position - vertices[i+2].position);
		float3 normalVector = math::Cross(line1, line2);
		normalVector.Normalize();

		float3 center;
		center.x = (vertices[i].position.x + vertices[i+1].position.x + vertices[i+2].position.x) / 3;
		center.y = (vertices[i].position.y + vertices[i+1].position.y + vertices[i+2].position.y) / 3;
		center.z = (vertices[i].position.z + vertices[i+1].position.z + vertices[i+2].position.z) / 3;

		glVertex3f(center.x, center.y, center.z);
		glVertex3f(center.x+normalVector.x, center.y + normalVector.y, center.z + normalVector.z);
	}
	glEnd();*/

	glBindVertexArray(0);
}

void Mesh::SetupMesh()
{
	// Generate VAO, VBO and EBO
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);

	// Bind all buffers and setup them
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	// VBO
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

	// EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

	// Setup all attributes for the VBO

	// Vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

	// Vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

	// Vertex normals
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));


	// Unbind VAO
	glBindVertexArray(0);
}
