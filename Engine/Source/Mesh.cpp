#include "Mesh.h"
#include "Globals.h"

#include "MeshImporter.h"
#include "Application.h"
#include "FileSystem.h"

#include "glew/include/GL/glew.h"

#include "Profiling.h"

Mesh::Mesh(uint uid, std::string& assets, std::string& library) : vbo(nullptr), ebo(nullptr), tbo(0), Resource(uid, ResourceType::MESH, assets, library)
{
	std::string metaPath = MESHES_FOLDER + std::string("mesh_") + std::to_string(uid) + ".meta";
	MeshImporter::CreateMetaMesh(metaPath, assets, uid);
	name = assets;
	app->fs->GetFilenameWithoutExtension(name);
	name = name.substr(name.find_first_of("__") + 2, name.length());
}

Mesh::~Mesh()
{
	vertices.clear();
	indices.clear();
	normals.clear();
	texCoords.clear();

	RELEASE(vbo);
	RELEASE(ebo);
	glDeleteBuffers(1, &tbo);
}

void Mesh::Load()
{
	if (vertices.empty())
	{
		MeshImporter::LoadMesh(vertices, indices, normals, texCoords, libraryPath);

		vbo = new VertexBuffer(vertices.data(), vertices.size() * sizeof(float3));
		ebo = new IndexBuffer(indices.data(), indices.size());

		glGenBuffers(1, &tbo);
		glBindBuffer(GL_ARRAY_BUFFER, tbo);
		glBufferData(GL_ARRAY_BUFFER, texCoords.size() * sizeof(float2), texCoords.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		vbo->Unbind();
		ebo->Unbind();
	}
}

void Mesh::UnLoad()
{
	if (!vertices.empty())
	{
		vertices.clear();
		indices.clear();
		normals.clear();
		texCoords.clear();

		glDeleteBuffers(1, &tbo);
		RELEASE(vbo);
		RELEASE(ebo);
	}
}

void Mesh::Draw(bool& verticesNormals, bool& faceNormals, float3& colorNormal, float& colorLength)
{
	vbo->Bind();
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	glBindBuffer(GL_ARRAY_BUFFER, tbo);
	glTexCoordPointer(2, GL_FLOAT, 0, NULL);

	if (ebo != nullptr) ebo->Bind();

	// Debug normals
	if (verticesNormals)
		ShowVertexNormals(colorNormal, colorLength);
	else if (faceNormals)
		ShowFaceNormals(colorNormal, colorLength);

	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

	if (ebo != nullptr) ebo->Unbind();
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	if (vbo != nullptr) vbo->Unbind();
}

void Mesh::ShowVertexNormals(float3& colorNormal, float &normalLength)
{
	if (!normals.empty())
	{
		glBegin(GL_LINES);
		glColor3f(colorNormal.x / 255, colorNormal.y / 255, colorNormal.z / 255);
		for (int i = 0; i < vertices.size(); ++i)
		{
			glVertex3f(vertices[i].x, vertices[i].y, vertices[i].z);
			float3 vertexNorm;
			vertexNorm.x = (vertices[i].x + (normals[i].x * normalLength));
			vertexNorm.y = (vertices[i].y + (normals[i].y * normalLength));
			vertexNorm.z = (vertices[i].z + (normals[i].z * normalLength));
			glVertex3f(vertexNorm.x, vertexNorm.y, vertexNorm.z);
		}
		glColor3f(1, 1, 1);
		glEnd();
	}
}

void Mesh::ShowFaceNormals(float3& colorNormal, float& normalLength)
{
	glBegin(GL_LINES);
	glColor3f(colorNormal.x / 255, colorNormal.y / 255, colorNormal.z / 255);
	int result = vertices.size() % 3;
	int size = vertices.size();
	if (result != 0) size -= result;
	for (int i = 0; i < size; i += 3)
	{
		float3 line1 = -(vertices[i] - vertices[i + 1]);
		float3 line2 = (vertices[i + 1] - vertices[i + 2]);
		float3 normalVector = math::Cross(line1, line2);
		normalVector.Normalize();

		float3 center;
		center.x = (vertices[i].x + vertices[i + 1].x + vertices[i + 2].x) / 3;
		center.y = (vertices[i].y + vertices[i + 1].y + vertices[i + 2].y) / 3;
		center.z = (vertices[i].z + vertices[i + 1].z + vertices[i + 2].z) / 3;

		glVertex3f(center.x, center.y, center.z);
		glVertex3f(center.x + (normalVector.x * normalLength), center.y + (normalVector.y * normalLength), center.z + (normalVector.z * normalLength));
	}
	glColor3f(1, 1, 1);
	glEnd();
}

void Mesh::Reimport(ModelParameters& data)
{
	if (!vertices.empty())
	{
		vertices.clear();
		indices.clear();
		texCoords.clear();
		normals.clear();
	}
	MeshImporter::LoadMesh(vertices, indices, normals, texCoords, libraryPath);
}