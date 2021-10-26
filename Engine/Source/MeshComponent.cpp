#include "MeshComponent.h"
#include "TransformComponent.h"
#include "MaterialComponent.h"
#include "GameObject.h"
#include "Globals.h"

#include "Imgui/imgui.h"

#include "glew/include/GL/glew.h"

#include "mmgr/mmgr.h"

MeshComponent::MeshComponent(GameObject* own, TransformComponent* trans) : ebo(nullptr), vbo(nullptr), tbo(0), material(nullptr), transform(trans), faceNormals(false), verticesNormals(false)
{
	owner = own;
}

MeshComponent::MeshComponent(std::vector<float3>& vert, std::vector<unsigned int>& ind, MaterialComponent* mat, std::vector<float2>& texCoord) : vertices(vert), indices(ind), texCoords(texCoord), transform(nullptr), material(mat)
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

MeshComponent::MeshComponent(std::vector<float3>& vert, std::vector<unsigned int>& ind, std::vector<float2>& texCoord) : vertices(vert), indices(ind), texCoords(texCoord), transform(nullptr), material(nullptr)
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

	if (vbo != nullptr) vbo->Bind();
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	glBindBuffer(GL_ARRAY_BUFFER, tbo);
	glTexCoordPointer(2, GL_FLOAT, 0, NULL);

	if (material != nullptr) material->BindTexture();
	if (ebo != nullptr) ebo->Bind();

	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

	if (ebo != nullptr) ebo->Unbind();
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	if (vbo != nullptr) vbo->Unbind();
	if (material != nullptr) material->UnbindTexture();

	glPopMatrix();

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

void MeshComponent::OnEditor()
{
	if (ImGui::CollapsingHeader("Mesh Renderer"))
	{
		ImGui::Text("Number of vertices: ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%d", vertices.size());
		ImGui::Text("Number of indices: ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%d", indices.size());
		ImGui::Checkbox("Vertices normals", &verticesNormals);
		ImGui::Checkbox("Face normals", &faceNormals);
		ImGui::Separator();
	}
}

void MeshComponent::CreateAABB()
{
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