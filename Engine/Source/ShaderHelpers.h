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