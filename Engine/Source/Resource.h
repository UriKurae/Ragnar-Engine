#pragma once

typedef unsigned int uint;

enum class ResourceType
{
	NONE = 0,
	TEXTURE,
	MESH,
	MODEL
};

class Resource
{
public:
	Resource(uint id, ResourceType t) : uid(id), type(t){}

	~Resource() {}

	inline const ResourceType& GetType() const { return type; }

protected:
	uint uid;
	ResourceType type;
};