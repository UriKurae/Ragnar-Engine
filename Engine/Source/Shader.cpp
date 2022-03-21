#include "Shader.h"
#include "Application.h"
#include "Globals.h"

#include "FileSystem.h"
#include "GL/glew.h"

#include <fstream>

GLenum GetShaderTypeFromString(const std::string& type)
{
	if (type == "vertex")
		return GL_VERTEX_SHADER;
	else if (type == "fragment")
		return GL_FRAGMENT_SHADER;
	else
		DEBUG_LOG("Invalid Shader Type %s", type);
}

Shader::Shader(uint uid, std::string& assets, std::string& library) : Resource(uid, ResourceType::MESH, assets, library), rendererID(0)
{
	int s = assetsPath.find_last_of("/");
	int e = assetsPath.find_last_of(".");
	name = assetsPath.substr(s + 1, e - s - 1);

	source = "";

	struct _stat nowStat;
	if (_stat(assetsPath.c_str(), &nowStat) == 0)
	{
		lastStat = nowStat;
	}

	CreateMetaShader();

	//Load();
}

Shader::~Shader()
{
	glDeleteProgram(rendererID);
}

void Shader::Load()
{
	if (source == "")
	{
		source = ReadFile();

		auto shaderSources = SplitShaders(source);

		//CreateShader(shaderSources[GL_VERTEX_SHADER], shaderSources[GL_FRAGMENT_SHADER]);
		CreateShader(shaderSources[GL_VERTEX_SHADER], shaderSources[GL_FRAGMENT_SHADER]);

		created = true;
	}
}

void Shader::UnLoad()
{
	glDeleteProgram(rendererID);
}

void Shader::Bind()
{
	glUseProgram(rendererID);
}

void Shader::Unbind()
{
	glUseProgram(0);
}

void Shader::ReCompile(bool reReadSource)
{
	if(reReadSource)
		source = ReadFile();

	auto shaderSources = SplitShaders(source);

	//CreateShader(shaderSources[GL_VERTEX_SHADER], shaderSources[GL_FRAGMENT_SHADER]);
	CreateShader(shaderSources[GL_VERTEX_SHADER], shaderSources[GL_FRAGMENT_SHADER]);
}

void Shader::CreateMetaShader()
{
	JsonParsing metaFile;

	metaFile.SetNewJsonString(metaFile.ValueToObject(metaFile.GetRootValue()), "Assets Path", assetsPath.c_str());
	metaFile.SetNewJsonNumber(metaFile.ValueToObject(metaFile.GetRootValue()), "Uuid", uid);

	char* buffer = nullptr;
	size_t size = metaFile.Save(&buffer);

	std::string metaPath = SHADERS_FOLDER + std::string("shader_") + std::to_string(uid) + ".meta";
	app->fs->Save(metaPath.c_str(), buffer, size);

	RELEASE_ARRAY(buffer);
}

bool Shader::Refresh()
{
	struct _stat nowStat;
	if (_stat(assetsPath.c_str(), &nowStat) == 0)
	{
		if (lastStat.st_mtime != nowStat.st_mtime)
		{
			ctime_s(timeBuf, 26, &lastStat.st_mtime);
			printf("Time modified : %s", timeBuf);
			
			ReCompile();
			
			lastStat = nowStat;
			
			return true;
		}
	}

	return false;
}

char* Shader::GetLastModifiedDate()
{
	ctime_s(timeBuf, 26, &lastStat.st_mtime);
	return timeBuf;
}

void Shader::SetUniformBool(const std::string& name, bool b)
{
	GLint location = glGetUniformLocation(rendererID, name.c_str());
	if (location != -1)
		glUniform1f(location, b);
}

void Shader::SetUniform1i(const std::string& name, int i)
{
	GLint location = glGetUniformLocation(rendererID, name.c_str());
	if(location != -1)
		glUniform1i(location, i);
}

void Shader::SetUniform1f(const std::string& name, float f)
{
	GLint location = glGetUniformLocation(rendererID, name.c_str());
	if (location != -1)
		glUniform1f(location, f);
}

void Shader::SetUnifromVec2f(const std::string& name, float v0, float v1)
{
	GLint location = glGetUniformLocation(rendererID, name.c_str());
	if (location != -1)
		glUniform2f(location, v0, v1);
}

void Shader::SetUnifromVec2f(const std::string& name, float2 v)
{
	SetUnifromVec2f(name, v.x, v.y);
}

void Shader::SetUniformVec3f(const std::string& name, float v0, float v1, float v2)
{
	GLint location = glGetUniformLocation(rendererID, name.c_str());
	if (location != -1)
		glUniform3f(location, v0, v1, v2);
}

void Shader::SetUniformVec3f(const std::string& name, float3 v)
{
	SetUniformVec3f(name, v.x, v.y, v.z);
}

void Shader::SetUniformVec4f(const std::string& name, float v0, float v1, float v2, float v3)
{
	GLint location = glGetUniformLocation(rendererID, name.c_str());
	if (location != -1)
		glUniform4f(location, v0, v1, v2, v3);
}

void Shader::SetUniformVec4f(const std::string& name, float4 v)
{
	SetUniformVec4f(name, v.x, v.y, v.z, v.w);
}

void Shader::SetUniformMatrix3f(const std::string& name, const float3x3& mat)
{
	GLint location = glGetUniformLocation(rendererID, name.c_str());
	if (location != -1)
		glUniformMatrix3fv(location, 1, GL_FALSE, mat.ptr());
}

void Shader::SetUniformMatrix4f(const std::string& name, const float4x4& mat)
{
	GLint location = glGetUniformLocation(rendererID, name.c_str());
	if (location != -1)
		glUniformMatrix4fv(location, 1, GL_FALSE, mat.ptr());
}



void Shader::UpdateSourceCode(const std::string& newSource)
{
	source = newSource;
	ReCompile(false);
}

unsigned int Shader::CreateShader(const std::string& vertexSource, const std::string& fragmentSource)
{
	unsigned int vs = glCreateShader(GL_VERTEX_SHADER);
	const char* vsource = vertexSource.c_str();
	glShaderSource(vs, 1, &vsource, NULL);
	glCompileShader(vs);
	
	int success;
	glGetShaderiv(vs, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		char infoLog[512];
		glGetShaderInfoLog(vs, 512, NULL, infoLog);
		DEBUG_LOG("Vertex shader compilation failed: %s", infoLog);
	}


	unsigned int fs = glCreateShader(GL_FRAGMENT_SHADER);
	const char* fsource = fragmentSource.c_str();
	glShaderSource(fs, 1, &fsource, NULL);
	glCompileShader(fs);

	glGetShaderiv(fs, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		char infoLog[512];
		glGetShaderInfoLog(fs, 512, NULL, infoLog);
		DEBUG_LOG("Fragment shader compilation failed: %s", infoLog);
	}

	rendererID = glCreateProgram();
	glAttachShader(rendererID, vs);
	glAttachShader(rendererID, fs);

	glLinkProgram(rendererID);
	glValidateProgram(rendererID);
	
	glGetProgramiv(rendererID, GL_LINK_STATUS, &success);
	if (!success)
	{
		char infoLog[512];
		glGetProgramInfoLog(rendererID, 512, NULL, infoLog);
		DEBUG_LOG("Program Linking failed: %s", infoLog);
	}
	else
	{
		if (created)
		{
			std::string n = name = name.substr(0, name.find("."));
			DEBUG_LOG("%s Shader refreshed!", n.c_str());
		}
		else
		{
			DEBUG_LOG("Linked program %s successfully!", name.c_str());
		}

	}

	//glDetachShader(rendererID, vs);
	glDeleteShader(vs);

	//glDetachShader(rendererID, fs);
	glDeleteShader(fs);
	
	return 0;
}

std::string Shader::ReadFile()
{
	std::string ret;
	std::ifstream in(assetsPath, std::ios::in, std::ios::binary);

	if (in)
	{
		// We seek for the end of file so we can set the returning string size
		in.seekg(0, std::ios::end);
		ret.resize(in.tellg());

		// We now seek for the file beginning to put it at the string (passing the string's first position and its size)
		in.seekg(0, std::ios::beg);
		in.read(&ret[0], ret.size());

		// Finally close the file
		in.close();
	}
	
	return ret;
}

std::unordered_map<GLenum, std::string> Shader::SplitShaders(const std::string& source)
{
	std::unordered_map<GLenum, std::string> ret;

	const char* typeToken = "#type";
	size_t typeTokenLength = strlen(typeToken);
	size_t pos = source.find(typeToken, 0);

	while (pos != std::string::npos)
	{
		size_t lineEnding = source.find_first_of("\r\n", pos);

		size_t beginPos = pos + typeTokenLength + 1;
		std::string shaderType = source.substr(beginPos, lineEnding - beginPos);

		size_t nextLine = source.find_first_not_of("\r\n", lineEnding);
		pos = source.find(typeToken, nextLine);

		ret[GetShaderTypeFromString(shaderType)] = pos == std::string::npos ? source.substr(nextLine) : source.substr(nextLine, pos - nextLine);
	}

	return ret;
}
