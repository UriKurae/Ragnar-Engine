#pragma once

#include "Component.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "TextureBuffer.h"
#include "MathGeoLib/src/MathGeoLib.h"

#include <vector>

typedef unsigned int GLuint;
typedef unsigned char GLubyte;
class TransformComponent;
class MaterialComponent;
class Mesh;

class MeshComponent : public Component
{
public:
	MeshComponent(GameObject* own, TransformComponent* trans);
	~MeshComponent();

	void Draw() override;
	void OnEditor() override;

	bool OnLoad(JsonParsing& node) override;
	bool OnSave(JsonParsing& node, JSON_Array* array) override;

	void SetMesh(Mesh* m);
	void SetMesh(std::vector<float3>& vert, std::vector<unsigned int>& ind, std::vector<float2>& texCoord, std::vector<float3> norm, std::string& path);
	inline void SetTransform(TransformComponent* trans) { transform = trans; }
	inline void SetMaterial(MaterialComponent* mat) { material = mat; }

private:
	TransformComponent* transform;
	MaterialComponent* material;

	float normalLength;
	float3 colorNormal;
	
	bool faceNormals;
	bool verticesNormals;

	Mesh* mesh;

	AABB localBoundingBox;

	VertexBuffer* vboAabb;
	IndexBuffer* eboAabb;
};