#pragma once
#include "Resource.h"

#include <vector>
#include "Shader.h"
#include "Vertex.h"

class IndexBuffer;
class VertexBuffer;
class VertexArray;

struct ModelParameters;

class Mesh : public Resource
{
public:
	Mesh(uint uid, std::string& assets, std::string& library);
	~Mesh();

	void Load() override;
	void UnLoad() override;

	void Draw(bool& verticesNormals, bool& faceNormals, float3& colorNormal, float &colorLength);

	void ShowVertexNormals(float3& colorNormal, float& normalLength);
	void ShowFaceNormals(float3& colorNormal, float& normalLength);

	inline const char* GetPath() const { return path.c_str(); }
	inline const int& GetVerticesSize() const { return vertices.size(); }
	inline const Vertex* GetVerticesData() const { return vertices.data(); }
	const std::vector<float3> GetPositions();
	inline const std::vector<Vertex>& GetVerticesVector() const { return vertices; }
	inline const int& GetIndicesSize() const { return indices.size(); }
	inline const std::vector<unsigned int>& GetIndicesVector() const { return indices; }

	void Reimport(ModelParameters& data);

private:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	//std::vector<float3> normals;
	//std::vector<float2> texCoords;

	VertexArray* vertexArray;
	VertexBuffer* vbo;
	IndexBuffer* ebo;
	unsigned int tbo;
	Shader* shader;

	std::string path;
};