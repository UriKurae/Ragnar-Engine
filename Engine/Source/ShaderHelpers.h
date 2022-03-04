#pragma once
#include "GL/glew.h"
#include <stdint.h>

enum class ShaderDataType
{
	NONE = 0,
	FLOAT,
	VEC2F,
	VEC3F,
	VEC4F,
	MAT3,
	MAT4,
	INT,
	VEC2I,
	VEC3I,
	VEC4I,
	BOOL
};

static uint32_t GetShaderDataTypeSize(ShaderDataType type)
{
	switch (type)
	{
	case ShaderDataType::FLOAT:  return 4;
	case ShaderDataType::VEC2F:  return 4 * 2;
	case ShaderDataType::VEC3F:  return 4 * 3;
	case ShaderDataType::VEC4F:  return 4 * 4;
	case ShaderDataType::MAT3:	 return 4 * 3 * 3;
	case ShaderDataType::MAT4:	 return 4 * 4 * 4;
	case ShaderDataType::INT:	 return 4;
	case ShaderDataType::VEC2I:	 return 4 * 2;
	case ShaderDataType::VEC3I:	 return 4 * 3;
	case ShaderDataType::VEC4I:	 return 4 * 4;
	case ShaderDataType::BOOL:	 return 1;
	}
	return 0;
}

static GLenum GetOpenGLRawTypeFromShaderDataType(ShaderDataType type)
{
	switch (type)
	{
	case ShaderDataType::FLOAT:  return GL_FLOAT;
	case ShaderDataType::VEC2F:  return GL_FLOAT;
	case ShaderDataType::VEC3F:  return GL_FLOAT;
	case ShaderDataType::VEC4F:  return GL_FLOAT;
	case ShaderDataType::MAT3:	 return GL_FLOAT;
	case ShaderDataType::MAT4:	 return GL_FLOAT;
	case ShaderDataType::INT:	 return GL_INT;
	case ShaderDataType::VEC2I:	 return GL_INT;
	case ShaderDataType::VEC4I:	 return GL_INT;
	case ShaderDataType::VEC3I:	 return GL_INT;
	case ShaderDataType::BOOL:	 return GL_BOOL;
	}
	return 0;
}

namespace ShaderSources
{
	static const char* notLightSensibleShaderSource = "#type vertex\n"
		"#version 430\n\n"

		"layout(location = 0) in vec3 position;\n"
		"layout(location = 1) in vec3 normal;\n"
		"layout(location = 2) in vec2 texCoords;\n\n"

		"uniform mat4 model;\n"
		"uniform mat4 view;\n"
		"uniform mat4 projection;\n"
		"uniform mat3 normalMatrix;\n\n"

		"out vec3 vPosition;\n"
		"out vec4 vNormal;\n"
		"out vec2 vTexCoords;\n\n"

		"void main()\n"
		"{\n\t"
		"gl_Position = projection * view * model * vec4(position, 1);\n\n\t"

		"vTexCoords = texCoords;\n\t"
		"vPosition = vec3(model * vec4(position, 1));\n\t"
		"vNormal = normalMatrix * normal;\n\t"
		"vNormal = normalize((model * vec4(normal, 0.0)).xyz);\n"
		"}\n\n\n"


		"#type fragment\n\n"

		"in vec3 vPosition;\n"
		"in vec3 vNormal;\n"
		"in vec2 vTexCoords;\n\n"

		"out vec4 fragColor;\n\n"

		"void main()\n"
		"{\n\t"
		"fragColor = vec4(1.0f);\n"
		"}\n";


	//static std::string lightSensibleShaderSource = 

}