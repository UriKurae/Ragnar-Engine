#include "Application.h"
#include "MeshComponent.h"
#include "TransformComponent.h"
#include "MaterialComponent.h"
#include "GameObject.h"
#include "Globals.h"
#include "ModuleCamera3D.h"

#include "Mesh.h"
#include "LoadModel.h"
#include "Texture.h"

#include "Imgui/imgui.h"

#include "glew/include/GL/glew.h"

#include "Profiling.h"

MeshComponent::MeshComponent(GameObject* own, TransformComponent* trans) : material(nullptr), transform(trans), faceNormals(false), verticesNormals(false), normalLength(1.0f), colorNormal(150.0f, 0.0f, 255.0f), vboAabb(nullptr), eboAabb(nullptr)
{
	type = ComponentType::MESH_RENDERER;
	owner = own;
	mesh = nullptr;
}

MeshComponent::~MeshComponent()
{
	RELEASE(mesh);
}

void MeshComponent::Draw()
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glPushMatrix();
	/*glMultTransposeMatrixf(transform->GetTransform().ptr());*/
	glMultMatrixf(transform->GetTransform().Transposed().ptr());

	if (material != nullptr) material->BindTexture();
	
	if (mesh != nullptr && app->camera->ContainsAaBox(localBoundingBox) == 1 || app->camera->ContainsAaBox(localBoundingBox) == 2) mesh->Draw(verticesNormals, faceNormals, colorNormal, normalLength);
	if (material != nullptr) material->UnbindTexture();
	
	if (vboAabb && eboAabb)
	{
		vboAabb->Bind();
		glVertexPointer(3, GL_FLOAT, 0, NULL);
		eboAabb->Bind();
		glLineWidth(2.0f);
		glDrawElements(GL_LINES, eboAabb->GetSize(), GL_UNSIGNED_INT, NULL);
		glLineWidth(1.0f);
		vboAabb->Unbind();
		eboAabb->Unbind();
	}

	glPopMatrix();

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

void MeshComponent::OnEditor()
{
	ImGui::PushID(this);

	if (ImGui::CollapsingHeader("Mesh Renderer"))
	{
		Checkbox(this, "Active", active);
		ImGui::Text("Number of vertices: ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%d", mesh ? mesh->GetVerticesSize() : 0);
		ImGui::Text("Number of indices: ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%d", mesh ? mesh->GetIndicesSize() : 0);
		ImGui::Checkbox("Vertices normals", &verticesNormals);
		ImGui::Checkbox("Face normals", &faceNormals);
		ImGui::DragFloat("Normal Length", &normalLength, 0.200f);
		ImGui::DragFloat3("Normal Color", colorNormal.ptr(), 1.0f, 0.0f, 255.0f);
		ImGui::Separator();
	}

	ImGui::PopID();
}

bool MeshComponent::OnLoad(JsonParsing& node)
{
	RELEASE(mesh);
	LoadModel::GetInstance()->LoadMesh(node.GetJsonString("Path"), this);

	active = node.GetJsonBool("Active");

	return true;
}

bool MeshComponent::OnSave(JsonParsing& node, JSON_Array* array)
{
	JsonParsing file = JsonParsing();

	file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "Type", (int)type);
	file.SetNewJsonString(file.ValueToObject(file.GetRootValue()), "Path", mesh->GetPath());
	file.SetNewJsonBool(file.ValueToObject(file.GetRootValue()), "Active", active);

	node.SetValueToArray(array, file.GetRootValue());

	return true;
}

void MeshComponent::SetMesh(Mesh* m)
{
	mesh = m;
	localBoundingBox.SetNegativeInfinity();
	localBoundingBox.Enclose(mesh->GetVerticesData(), mesh->GetVerticesSize());

	owner->SetAABB(localBoundingBox);

	float3 corners[8];
	localBoundingBox.GetCornerPoints(corners);

	unsigned int indices[24] = {
	0,1,
	1,3,
	3,2,
	2,0,

	1,5,
	4,6,
	7,3,

	6,7,
	6,2,
	
	7,5,
	4,5,

	4,0
	};

	eboAabb = new IndexBuffer(indices, 24);
	vboAabb = new VertexBuffer(corners, sizeof(float3) * 8);
	eboAabb->Unbind();
	vboAabb->Unbind();

}

void MeshComponent::SetMesh(std::vector<float3>& vert, std::vector<unsigned int>& ind, std::vector<float2>& texCoord, std::vector<float3> norm, std::string& path)
{
	RELEASE(mesh);
	mesh = new Mesh(vert, ind, norm, texCoord, path);
	localBoundingBox.SetNegativeInfinity();
	localBoundingBox.Enclose(vert.data(), vert.size());

	owner->SetAABB(localBoundingBox);

	float3 corners[8];
	localBoundingBox.GetCornerPoints(corners);

	unsigned int indices[24] = {
	0,1,
	1,3,
	3,2,
	2,0,

	1,5,
	4,6,
	7,3,

	6,7,
	6,2,

	7,5,
	4,5,

	4,0
	};

	eboAabb = new IndexBuffer(indices, 24);
	vboAabb = new VertexBuffer(corners, sizeof(float3) * 8);
	eboAabb->Unbind();
	vboAabb->Unbind();
}