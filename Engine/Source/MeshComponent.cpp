#include "MeshComponent.h"
#include "TransformComponent.h"
#include "MaterialComponent.h"
#include "GameObject.h"
#include "Globals.h"

#include "Mesh.h"
#include "LoadModel.h"

#include "Imgui/imgui.h"

#include "glew/include/GL/glew.h"

#include "Profiling.h"

MeshComponent::MeshComponent(GameObject* own, TransformComponent* trans) : ebo(nullptr), vbo(nullptr), tbo(0), material(nullptr), transform(trans), faceNormals(false), verticesNormals(false), normals({}), normalLength(1.0f), colorNormal(150.0f, 0.0f, 255.0f)
{
	type = ComponentType::MESH_RENDERER;
	owner = own;
	mesh = nullptr;
}

MeshComponent::MeshComponent(std::vector<float3>& vert, std::vector<unsigned int>& ind, MaterialComponent* mat, std::vector<float2>& texCoord) : vertices(vert), indices(ind), texCoords(texCoord), transform(nullptr), material(mat), normals({}), normalLength(1.0f), colorNormal(150.0f, 0.0f, 255.0f)
{
	type = ComponentType::MESH_RENDERER;
	verticesNormals = false;
	faceNormals = false;

	vbo = new VertexBuffer(vertices.data(), vertices.size() * sizeof(float3));
	ebo = new IndexBuffer(indices.data(), indices.size());
	glGenBuffers(1, &tbo);
	glBindBuffer(GL_ARRAY_BUFFER, tbo);
	glBufferData(GL_ARRAY_BUFFER, texCoords.size() * sizeof(float2), texCoords.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	vbo->Unbind();
	ebo->Unbind();
}

MeshComponent::MeshComponent(std::vector<float3>& vert, std::vector<unsigned int>& ind, std::vector<float2>& texCoord) : vertices(vert), indices(ind), texCoords(texCoord), transform(nullptr), material(nullptr), normals({}), normalLength(1.0f), colorNormal(75.0f, 0.0f, 127.0f)
{
	type = ComponentType::MESH_RENDERER;
	verticesNormals = false;
	faceNormals = false;

	vbo = new VertexBuffer(vertices.data(), vertices.size() * sizeof(float3));
	ebo = new IndexBuffer(indices.data(), indices.size());
	glGenBuffers(1, &tbo);
	glBindBuffer(GL_ARRAY_BUFFER, tbo);
	glBufferData(GL_ARRAY_BUFFER, texCoords.size() * sizeof(float2), texCoords.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	vbo->Unbind();
	ebo->Unbind();
}

MeshComponent::MeshComponent(std::vector<float3>& vert, std::vector<unsigned int>& ind, std::vector<float2>& texCoord, std::vector<float3>& norm)
{
	vertices = vert;
	indices = ind;
	normals = norm;
	texCoords = texCoord;

	type = ComponentType::MESH_RENDERER;
	verticesNormals = false;
	faceNormals = false;

	vbo = new VertexBuffer(vertices.data(), vertices.size() * sizeof(float3));
	ebo = new IndexBuffer(indices.data(), indices.size());
	glGenBuffers(1, &tbo);
	glBindBuffer(GL_ARRAY_BUFFER, tbo);
	glBufferData(GL_ARRAY_BUFFER, texCoords.size() * sizeof(float2), texCoords.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	vbo->Unbind();
	ebo->Unbind();
}

MeshComponent::~MeshComponent()
{
	RELEASE(vbo);
	RELEASE(ebo);
}

void MeshComponent::Draw()
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glPushMatrix();
	/*glMultTransposeMatrixf(transform->GetTransform().ptr());*/
	glMultMatrixf(transform->GetTransform().Transposed().ptr());

	if (mesh != nullptr) mesh->Draw(verticesNormals, faceNormals);
	
	//if (vbo != nullptr) vbo->Bind();
	//glVertexPointer(3, GL_FLOAT, 0, NULL);

	//glBindBuffer(GL_ARRAY_BUFFER, tbo);
	//glTexCoordPointer(2, GL_FLOAT, 0, NULL);

	//if (material != nullptr && material->GetActive()) material->BindTexture();
	//if (ebo != nullptr) ebo->Bind();

	//// Debug normals
	//if (verticesNormals)
	//	ShowVertexNormals();
	//else if (faceNormals)
	//	ShowFaceNormals();

	//glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

	//if (ebo != nullptr) ebo->Unbind();
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	//if (vbo != nullptr) vbo->Unbind();
	//if (material != nullptr && material->GetActive()) material->UnbindTexture();
	
	glPopMatrix();

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

void MeshComponent::OnEditor()
{
	if (ImGui::CollapsingHeader("Mesh Renderer"))
	{
		Checkbox(this, "Active", active);
		ImGui::Text("Number of vertices: ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%d", vertices.size());
		ImGui::Text("Number of indices: ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%d", indices.size());
		ImGui::Checkbox("Vertices normals", &verticesNormals);
		ImGui::Checkbox("Face normals", &faceNormals);
		ImGui::DragFloat("Normal Length", &normalLength, 0.200f);
		ImGui::DragFloat3("Normal Color", colorNormal.ptr(), 1.0f, 0.0f, 255.0f);
		ImGui::Separator();
	}
}

bool MeshComponent::OnLoad(JsonParsing& node)
{
	LoadModel::GetInstance()->LoadMesh(node.GetJsonString("Path"), this);

	return true;
}

bool MeshComponent::OnSave(JsonParsing& node, JSON_Array* array)
{
	JsonParsing file = JsonParsing();

	file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "Type", (int)type);
	file.SetNewJsonString(file.ValueToObject(file.GetRootValue()), "Path", mesh->GetPath());

	node.SetValueToArray(array, file.GetRootValue());

	return true;
}

void MeshComponent::CreateAABB()
{
	owner->SetAABB(vertices);
}

void MeshComponent::ShowVertexNormals()
{
	if (!normals.empty())
	{
		glBegin(GL_LINES);
		glColor3f(colorNormal.x/255, colorNormal.y/255, colorNormal.z/255);
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

void MeshComponent::ShowFaceNormals()
{
	glBegin(GL_LINES);
	glColor3f(colorNormal.x / 255, colorNormal.y / 255, colorNormal.z / 255);
	for (int i = 0; i < vertices.size(); i += 3)
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

void MeshComponent::SetMesh(std::vector<float3>& vert, std::vector<unsigned int>& ind, std::vector<float2>& texCoord, std::vector<float3>& norm)
{
	type = ComponentType::MESH_RENDERER;
	verticesNormals = false;
	faceNormals = false;

	normals = norm;
	vertices = vert;
	indices = ind;
	texCoords = texCoord;

	vbo = new VertexBuffer(vertices.data(), vertices.size() * sizeof(float3));
	ebo = new IndexBuffer(indices.data(), indices.size());
	glGenBuffers(1, &tbo);
	glBindBuffer(GL_ARRAY_BUFFER, tbo);
	glBufferData(GL_ARRAY_BUFFER, texCoords.size() * sizeof(float2), texCoords.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	vbo->Unbind();
	ebo->Unbind();

	owner->SetAABB(vertices);
}

void MeshComponent::SetMesh(std::vector<float3>& vert, std::vector<unsigned int>& ind, std::vector<float2>& texCoord)
{
	type = ComponentType::MESH_RENDERER;
	verticesNormals = false;
	faceNormals = false;

	vertices = vert;
	indices = ind;
	texCoords = texCoord;

	vbo = new VertexBuffer(vertices.data(), vertices.size() * sizeof(float3));
	ebo = new IndexBuffer(indices.data(), indices.size());
	glGenBuffers(1, &tbo);
	glBindBuffer(GL_ARRAY_BUFFER, tbo);
	glBufferData(GL_ARRAY_BUFFER, texCoords.size() * sizeof(float2), texCoords.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	vbo->Unbind();
	ebo->Unbind();

	owner->SetAABB(vertices);
}