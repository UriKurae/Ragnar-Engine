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

	//RELEASE(vertexArray);
	RELEASE(vbo);
	RELEASE(ebo);
}

void Mesh::Load()
{
	if (vertices.empty())
	{
		MeshImporter::LoadMesh(vertices, indices, bones, libraryPath);

		numBones = bones.size();

		vertexArray = new VertexArray();

		vbo = new VertexBuffer(vertices);
		vbo->SetLayout({
			{ShaderDataType::VEC3F, "position"},
			{ShaderDataType::VEC3F, "normal"},
			{ShaderDataType::VEC2F, "texCoords"},
			{ShaderDataType::VEC4I, "boneIds"},
			{ShaderDataType::VEC4F, "weights"}
		});
		vertexArray->AddVertexBuffer(*vbo);

		ebo = new IndexBuffer(indices.data(), indices.size());
		vertexArray->SetIndexBuffer(*ebo);
	}
}

void Mesh::UnLoad()
{
	if (!vertices.empty())
	{
		vertices.clear();
		indices.clear();

		//glDeleteBuffers(1, &tbo);
		RELEASE(vbo);
		RELEASE(ebo);
		RELEASE(vertexArray);
	}
}

void Mesh::Draw(bool& verticesNormals, bool& faceNormals, float3& colorNormal, float& colorLength)
{
	//vbo->Bind();
	//vertexArray->Bind();
	//glVertexPointer(3, GL_FLOAT, sizeof(Vertex), NULL);
	//
	//glBindBuffer(GL_ARRAY_BUFFER, tbo);
	//glTexCoordPointer(2, GL_FLOAT, 0, NULL);
	//
	//if (ebo != nullptr) ebo->Bind();

	//vbo->Bind();
	//ebo->Bind();
	//shader->Bind();
	
	vertexArray->Bind();
	
	if (verticesNormals)
		ShowVertexNormals(colorNormal, colorLength);
	else if (faceNormals)
		ShowFaceNormals(colorNormal, colorLength);

	
	unsigned int s = vertexArray->GetIndexBuffer()->GetCount();
	//vbo->Bind();
	ebo->Bind();
	glDrawElements(GL_TRIANGLES, s, GL_UNSIGNED_INT, 0);

	//glDrawArrays(GL_TRIANGLES, 0, vertices.size());

	vertexArray->Unbind();
	
	// Debug normals
	
	
	//if (ebo != nullptr) ebo->Unbind();
	////glBindBuffer(GL_ARRAY_BUFFER, 0);
	//if (vbo != nullptr) vbo->Unbind();
}

void Mesh::ShowVertexNormals(float3& colorNormal, float &normalLength)
{
	if (!vertices.empty())
	{
		glBegin(GL_LINES);
		glColor3f(colorNormal.x / 255, colorNormal.y / 255, colorNormal.z / 255);
		for (int i = 0; i < vertices.size(); ++i)
		{
			glVertex3f(vertices[i].position.x, vertices[i].position.y, vertices[i].position.z);
			float3 vertexNorm;
			vertexNorm.x = (vertices[i].position.x + (vertices[i].normal.x * normalLength));
			vertexNorm.y = (vertices[i].position.y + (vertices[i].normal.y * normalLength));
			vertexNorm.z = (vertices[i].position.z + (vertices[i].normal.z * normalLength));
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
		float3 line1 = -(vertices[i].position - vertices[i + 1].position);
		float3 line2 = (vertices[i + 1].position - vertices[i + 2].position);
		float3 normalVector = math::Cross(line1, line2);
		normalVector.Normalize();

		float3 center;
		center.x = (vertices[i].position.x + vertices[i + 1].position.x + vertices[i + 2].position.x) / 3;
		center.y = (vertices[i].position.y + vertices[i + 1].position.y + vertices[i + 2].position.y) / 3;
		center.z = (vertices[i].position.z + vertices[i + 1].position.z + vertices[i + 2].position.z) / 3;

		glVertex3f(center.x, center.y, center.z);
		glVertex3f(center.x + (normalVector.x * normalLength), center.y + (normalVector.y * normalLength), center.z + (normalVector.z * normalLength));
	}
	glColor3f(1, 1, 1);
	glEnd();
}

const std::vector<float3> Mesh::GetPositions()
{
	std::vector<float3> positions = {};
	positions.reserve(vertices.size());

	for (int i = 0; i < vertices.size(); ++i)
		positions.emplace_back(vertices[i].position);

	return positions;
}

void Mesh::Reimport(ModelParameters& data)
{
	if (!vertices.empty())
	{
		vertices.clear();
		indices.clear();
		bones.clear();
		//texCoords.clear();
		//normals.clear();
	}
	MeshImporter::LoadMesh(vertices, indices, bones, libraryPath);
}