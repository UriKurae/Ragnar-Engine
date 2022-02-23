#include "Application.h"
#include "Globals.h"
#include "MeshComponent.h"

#include "ModuleScene.h"
#include "CameraComponent.h"
#include "FileSystem.h"
#include "ResourceManager.h"

#include "Mesh.h"

#include "Imgui/imgui.h"

#include "glew/include/GL/glew.h"

#include "IconsFontAwesome5.h"

#include "Profiling.h"

MeshComponent::MeshComponent(GameObject* own, TransformComponent* trans) : material(nullptr), transform(trans), faceNormals(false), verticesNormals(false), normalLength(1.0f), colorNormal(150.0f, 0.0f, 255.0f)
{
	type = ComponentType::MESH_RENDERER;
	owner = own;
	mesh = nullptr;
	material = owner->GetComponent<MaterialComponent>();

	showMeshMenu = false;
}

MeshComponent::MeshComponent(MeshComponent* meshComponent, TransformComponent* trans) : material(nullptr), showMeshMenu(false)
{
	transform = trans;
	mesh = meshComponent->GetMesh();

	faceNormals = meshComponent->faceNormals;
	verticesNormals = meshComponent->verticesNormals;
	normalLength = meshComponent->normalLength;
	colorNormal = meshComponent->colorNormal;

	localBoundingBox = meshComponent->localBoundingBox;
}

MeshComponent::~MeshComponent()
{
	if (mesh.use_count() - 1 == 1) mesh->UnLoad();
}

void MeshComponent::Draw(CameraComponent* gameCam)
{
	//glEnableClientState(GL_VERTEX_ARRAY);
	//glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	//glPushMatrix();
	//glMultMatrixf(transform->GetGlobalTransform().Transposed().ptr());
	
	
	if (material != nullptr && material->GetActive()) material->Bind(gameCam);
	
	if (mesh != nullptr) mesh->Draw(verticesNormals, faceNormals, colorNormal, normalLength);
	
	if (material != nullptr && material->GetActive()) material->Unbind();
	
	//glPopMatrix();
	//
	//glDisableClientState(GL_VERTEX_ARRAY);
	//glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	// If showAABB are enable draw the his bounding boxes
	if (showAABB == true) {
		float3 points[8];
		owner->GetAABB().GetCornerPoints(points);
		DebugColliders(points, float3(0.2f, 1.f, 0.101f));
	}
	// If showOBB are enable draw the his bounding boxes
	if (showOBB == true) {
		float3 points[8];
		owner->GetOOB().GetCornerPoints(points);
		DebugColliders(points);
	}
}

void MeshComponent::DebugColliders(float3* points, float3 color)
{
	unsigned int index[24] =
	{ 0, 2, 2, 6, 6, 4, 4, 0,
	  0, 1, 1, 3, 3, 2, 4, 5,
	  6, 7, 5, 7, 3, 7, 1, 5
	};

	glColor3fv(&color.x);
	glLineWidth(2.f);
	glBegin(GL_LINES);

	for (int i = 0; i < 24; i++)
	{
		glVertex3fv(&points[index[i]].x);
	}

	glEnd();
	glLineWidth(1.f);
	glColor3f(1.f, 1.f, 1.f);
}

void MeshComponent::DrawOutline()
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glPushMatrix();
	float4x4 testGlobal = transform->GetGlobalTransform();

	testGlobal.scaleX += 0.05f;
	testGlobal.scaleY += 0.05f;
	testGlobal.scaleZ += 0.05f;
	glMultMatrixf(testGlobal.Transposed().ptr());

	if (mesh != nullptr) mesh->Draw(verticesNormals, faceNormals, colorNormal, normalLength);

	glPopMatrix();
	
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

void MeshComponent::OnEditor()
{
	ImGui::PushID(this);

	if (ImGui::CollapsingHeader(ICON_FA_CUBES" Mesh Renderer"))
	{
		Checkbox(this, "Active", active);
		ImGui::Text("Select mesh");
		ImGui::SameLine();
		if (ImGui::Button(mesh ? mesh->GetName().c_str() : "No Mesh"))
		{
			showMeshMenu = true;
		}
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
		ImGui::Text("Reference Count: ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%d", mesh ? mesh.use_count() : 0);

		ImGui::Checkbox("Show AABB     ", &showAABB);		
		ImGui::SameLine();		
		ImGui::Checkbox("Show OBB", &showOBB);

		ImGui::SetCursorPosX(ImGui::GetWindowContentRegionMax().x - ImGui::CalcTextSize("Delete").x - 25);
		if (ImGui::Button(ICON_FA_TRASH" Delete"))
			owner->RemoveComponent(this);

		ImGui::Separator();
	}

	if (showMeshMenu)
	{
		ImGui::Begin("Meshes", &showMeshMenu, ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse);
		ImVec2 winPos = ImGui::GetWindowPos();
		ImVec2 size = ImGui::GetWindowSize();
		ImVec2 mouse = ImGui::GetIO().MousePos;
		if (!(mouse.x < winPos.x + size.x && mouse.x > winPos.x && 
			mouse.y < winPos.y + size.y && mouse.y > winPos.y))
		{
			if (ImGui::GetIO().MouseClicked[0]) showMeshMenu = false;
		}

		std::vector<std::string> files;
		app->fs->DiscoverFiles("Library/Meshes/", files);
		for (std::vector<std::string>::iterator it = files.begin(); it != files.end(); ++it)
		{
			if ((*it).find(".rgmesh") != std::string::npos)
			{
				app->fs->GetFilenameWithoutExtension(*it);
				*it = (*it).substr((*it).find_last_of("_") + 1, (*it).length());
				uint uid = std::stoll(*it);
				std::shared_ptr<Resource> res = ResourceManager::GetInstance()->LoadResource(uid);
				if (ImGui::Selectable(res->GetName().c_str()))
				{
					if (mesh.use_count() - 1 == 1) mesh->UnLoad();
					SetMesh(res);
				}
			}
		}

		ImGui::End();
	}

	ImGui::PopID();
}

bool MeshComponent::OnLoad(JsonParsing& node)
{
	mesh = std::static_pointer_cast<Mesh>(ResourceManager::GetInstance()->LoadResource(std::string(node.GetJsonString("Path"))));

	active = node.GetJsonBool("Active");

	if (mesh)
	{
		localBoundingBox.SetNegativeInfinity();

		std::vector<float3> positions = {};
		positions.resize(mesh->GetVerticesSize());

		for (int i = 0; i < mesh->GetVerticesSize(); ++i)
			positions[i] = mesh->GetVerticesData()->position;

		localBoundingBox.Enclose(positions.data(), positions.size());

		owner->SetAABB(localBoundingBox);
	}

	return true;
}

bool MeshComponent::OnSave(JsonParsing& node, JSON_Array* array)
{
	JsonParsing file = JsonParsing();

	file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "Type", (int)type);
	file.SetNewJsonString(file.ValueToObject(file.GetRootValue()), "Path", mesh->GetAssetsPath().c_str());
	file.SetNewJsonBool(file.ValueToObject(file.GetRootValue()), "Active", active);

	node.SetValueToArray(array, file.GetRootValue());

	return true;
}

void MeshComponent::SetMesh(std::shared_ptr<Resource> m)
{
	mesh = std::static_pointer_cast<Mesh>(m);

	if (mesh)
	{
		localBoundingBox.SetNegativeInfinity();

		std::vector<float3> positions = {};
		positions.resize(mesh->GetVerticesSize());

		for (int i = 0; i < mesh->GetVerticesSize(); ++i)
			positions[i] = mesh->GetVerticesData()->position;

		localBoundingBox.Enclose(positions.data(), positions.size());

		owner->SetAABB(localBoundingBox);
	}
}

bool MeshComponent::HasMaterial()
{
	if (material) return true;
	
	return false;
}