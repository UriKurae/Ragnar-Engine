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

struct Texture
{
	GLuint id;
	const char* type;
	int width;
	int height;
	GLubyte* data;
};

class MeshComponent : public Component
{
public:
	MeshComponent(GameObject* own, TransformComponent* trans);
	MeshComponent(std::vector<float3>& vert, std::vector<unsigned int>& ind, MaterialComponent* material, std::vector<float2>& texCoord);
	MeshComponent(std::vector<float3>& vert, std::vector<unsigned int>& ind, std::vector<float2>& texCoord);
	MeshComponent(std::vector<float3>& vert, std::vector<unsigned int>& ind, std::vector<float2>& texCoord, std::vector<float3>& norm);
	~MeshComponent();

	void Draw() override;
	void OnEditor() override;

	bool OnLoad(JsonParsing& node, JSON_Array* array) override;
	bool OnSave(JsonParsing& node, JSON_Array* array) override;

	void CreateAABB();
	
	void ShowVertexNormals();
	void ShowFaceNormals();

	void SetMesh(std::vector<float3>& vert, std::vector<unsigned int>& ind, std::vector<float2>& texCoord, std::vector<float3>& norm);
	void SetMesh(std::vector<float3>& vert, std::vector<unsigned int>& ind, std::vector<float2>& texCoord);
	inline void SetTransform(TransformComponent* trans) { transform = trans; }
	inline void SetMaterial(MaterialComponent* mat) { material = mat; }

private:
	VertexBuffer* vbo;
	IndexBuffer* ebo;
	//Texture texture;
	unsigned int tbo;

	std::vector<float3> vertices;
	std::vector<float3> normals;
	std::vector<unsigned int> indices;
	std::vector<float2> texCoords;

	TransformComponent* transform;
	MaterialComponent* material;

	float normalLength;
	float3 colorNormal;
	
	bool faceNormals;
	bool verticesNormals;
};