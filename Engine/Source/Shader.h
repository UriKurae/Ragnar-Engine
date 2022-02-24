#pragma once

#include "Resource.h"

#include "Math/float4x4.h"
#include "Math/float3x3.h"
#include "Math/float2.h"

#include <unordered_map>
#include <string>

enum class ShaderType
{
	FRAGMENT = 0,
	VERTEX,
	GEOMETRY
};

struct UniformData
{
	unsigned int type;
	float data;
	std::string name;
};

class Shader : public Resource
{
public:
	Shader(uint uid, std::string& assets, std::string& library);
	virtual ~Shader();

	void Load() override;
	void UnLoad() override;

	void Bind();
	void Unbind();

	bool Refresh();

	inline unsigned int GetId() { return rendererID; }

	inline const std::string& GetName() { return name; }
	inline void SetName(const std::string n) { name = n; }
	char* GetLastModifiedDate();

	void SetUniformBool(const std::string& name, bool b);

	void SetUniform1i(const std::string& name, int i);
	void SetUniform1f(const std::string& name, float f);

	void SetUnifromVec2f(const std::string& name, float v0, float v1);
	void SetUnifromVec2f(const std::string& name, float2 v);

	void SetUniformVec3f(const std::string& name, float v0, float v1, float v2);
	void SetUniformVec3f(const std::string& name, float3 v);

	void SetUniformVec4f(const std::string& name, float v0, float v1, float v2, float v3);
	void SetUniformVec4f(const std::string& name, float4 v);

	void SetUniformMatrix3f(const std::string& name, const float3x3& mat);
	void SetUniformMatrix4f(const std::string& name, const float4x4& mat);

	//inline const std::string& GetPath() { return path; }
	inline const std::string& GetSource() { return source; }

	std::list<UniformData> GetUniforms();
	void UpdateSourceCode(const std::string& newSource);

private:
	unsigned int CreateShader(const std::string& vertexSource, const std::string& fragmentSource);
	std::string ReadFile();
	std::unordered_map<unsigned int, std::string> SplitShaders(const std::string& source);

	void ReCompile(bool reReadSource = true);

private:
	std::string name;
	unsigned int rendererID;
	std::string source;
	//std::string path;

	bool created;

	std::unordered_map<std::string, int> uniformsLocation;

	struct _stat lastStat;
	char timeBuf[26];
};