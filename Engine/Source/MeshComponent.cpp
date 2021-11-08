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
}

void MeshComponent::SetMesh(std::vector<float3>& vert, std::vector<unsigned int>& ind, std::vector<float2>& texCoord, std::vector<float3> norm, std::string& path)
{
	RELEASE(mesh);
	mesh = new Mesh(vert, ind, norm, texCoord, path);
	localBoundingBox.SetNegativeInfinity();
	localBoundingBox.Enclose(vert.data(), vert.size());

	owner->SetAABB(localBoundingBox);
}