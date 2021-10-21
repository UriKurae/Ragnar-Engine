#include "MeshComponent.h"
#include "TransformComponent.h"
#include "Globals.h"

#include "Imgui/imgui.h"

#include "glew/include/GL/glew.h"

#include "mmgr/mmgr.h"

MeshComponent::MeshComponent(std::vector<float3> vert, std::vector<unsigned int> ind, MaterialComponent* mat, std::vector<float2> texCoord) : vertices(vert), indices(ind), texCoords(texCoord), transform(nullptr), material(mat)
{
	type = ComponentType::MESH_RENDERER;

	vbo = new VertexBuffer(vertices.data(), vertices.size() * sizeof(float3));
	ebo = new IndexBuffer(indices.data(), indices.size());
	glGenBuffers(1, &tbo);
	glBindBuffer(GL_ARRAY_BUFFER, tbo);
	glBufferData(GL_ARRAY_BUFFER, texCoords.size() * sizeof(float2), texCoords.data(), GL_STATIC_DRAW);

	texBuffer = new TextureBuffer(material);

	texBuffer->Unbind();
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	vbo->Unbind();
	ebo->Unbind();
}

MeshComponent::~MeshComponent()
{
	RELEASE(vbo);
	RELEASE(ebo);
	RELEASE(texBuffer);
}

void MeshComponent::Draw()
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glPushMatrix();
	glMultMatrixf(transform->GetTransform());

	vbo->Bind();
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	glBindBuffer(GL_ARRAY_BUFFER, tbo);
	glTexCoordPointer(2, GL_FLOAT, 0, NULL);

	texBuffer->Bind();
	ebo->Bind();

	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

	ebo->Unbind();
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	vbo->Unbind();
	texBuffer->Unbind();

	glPopMatrix();

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

void MeshComponent::OnEditor()
{
	//if (ImGui::CollapsingHeader("Mesh Renderer"))
	//{
	//	ImGui::SameLine();
	//	ImGui::Checkbox("", &active);
	//}
}