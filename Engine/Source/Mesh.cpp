#include "Mesh.h"
#include "Globals.h"

#include "glew/include/GL/glew.h"

#include "Profiling.h"

Mesh::Mesh(std::vector<float3>& vert, std::vector<unsigned int>& ind, std::vector<float3>& norm, std::vector<float2>& texCoord, std::string& p)
{
	vertices = vert;
	indices = ind;
	normals = norm;
	texCoords = texCoord;
	path = p;

	vbo = new VertexBuffer(vertices.data(), vertices.size() * sizeof(float3));
	ebo = new IndexBuffer(indices.data(), indices.size());

	glGenBuffers(1, &tbo);
	glBindBuffer(GL_ARRAY_BUFFER, tbo);
	glBufferData(GL_ARRAY_BUFFER, texCoords.size() * sizeof(float2), texCoords.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	vbo->Unbind();
	ebo->Unbind();
}

Mesh::~Mesh()
{
	vertices.clear();
	indices.clear();
	normals.clear();
	texCoords.clear();

	RELEASE(vbo);
	RELEASE(ebo);
}

void Mesh::Draw(bool& verticesNormals, bool& faceNormals)
{
	vbo->Bind();
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	glBindBuffer(GL_ARRAY_BUFFER, tbo);
	glTexCoordPointer(2, GL_FLOAT, 0, NULL);

	//if (material != nullptr && material->GetActive()) material->BindTexture();
	if (ebo != nullptr) ebo->Bind();

	// Debug normals
	if (verticesNormals)
		ShowVertexNormals();
	else if (faceNormals)
		ShowFaceNormals();

	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

	if (ebo != nullptr) ebo->Unbind();
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	if (vbo != nullptr) vbo->Unbind();
	//if (material != nullptr && material->GetActive()) material->UnbindTexture();
}

void Mesh::ShowVertexNormals()
{
}

void Mesh::ShowFaceNormals()
{
}