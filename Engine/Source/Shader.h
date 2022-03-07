#pragma once
#include "Resource.h"

#include "Math/float4x4.h"
#include "Math/float3x3.h"
#include "Math/float2.h"
#include "GL/glew.h"

#include <unordered_map>

enum class UniformType
{
	INT,
	FLOAT,
	BOOL,
	INT_VEC2,
	INT_VEC3,
	INT_VEC4,
	FLOAT_VEC2,
	FLOAT_VEC3,
	FLOAT_VEC4,
	MATRIX4,
	NONE
};
struct Uniform
{
	UniformType uniformType = UniformType::NONE;
	std::string name;
	GLenum GLtype;

	bool boolean;
	GLint integer;
	GLfloat floatNumber;
	float2 vec2;
	float3 vec3;
	float4 vec4;
	float4x4 matrix4 = float4x4::zero;

};

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
	inline const std::vector<Uniform>& GetUniforms() { return uniforms; }
	void SetUniforms(const std::vector<Uniform>& u) { uniforms = u; }
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

	void UpdateSourceCode(const std::string& newSource);

private:
	unsigned int CreateShader(const std::string& vertexSource, const std::string& fragmentSource);
	std::string ReadFile();
	std::unordered_map<unsigned int, std::string> SplitShaders(const std::string& source);

	void ReCompile(bool reReadSource = true);

	void CreateMetaShader();

private:
	std::string name;
	unsigned int rendererID;
	std::string source;
	//std::string path;
	std::vector<Uniform> uniforms;

	bool created;

	std::unordered_map<std::string, int> uniformsLocation;

	struct _stat lastStat;
	char timeBuf[26];
};