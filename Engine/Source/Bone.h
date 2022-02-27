#pragma once

#include "Resource.h"
#include "MathGeoLib/src/MathGeoLib.h"

struct BoneInfo
{
	int id;
	float4x4 offset;
};

struct KeyPosition
{
	float3 position;
	float timeStamp;
};

struct KeyRotation
{
	Quat orientation;
	float timeStamp;
};

struct KeyScale
{
	float3 scale;
	float timeStamp;
};

class Bone
{
public:
	Bone(){}
	Bone(std::vector<KeyPosition> bonePositions, std::vector<KeyRotation> boneRotations, std::vector<KeyScale> boneScales, int boneId, std::string boneName);

private:
	std::vector<KeyPosition> positions;
	std::vector<KeyRotation> rotations;
	std::vector<KeyScale> scales;
	int numPositions;
	int numRotations;
	int numScalings;

	math::float4x4 localTransform;
	std::string name;
	int id;
};




struct Weight 
{
public:
	Weight(unsigned int v, float w) : vertexId(v), weight(w) {}

	unsigned int vertexId;
	float weight;
};

class Bone2 : public Resource
{
public:
	Bone2(uint uid, std::string& assets, std::string& library);
	~Bone2();

	void Load() override;
	//void UnLoad() override;

	inline const std::vector<Weight> GetWeight() const { return weights; }
	inline const int& GetNumWeights() const { return numWeights; }
	inline const float4x4& GetOffset() const { return offset; }
	inline const float* GetPosition() const { return position; }
	inline const float* GetRotation() const { return rotation; }
	inline const float* GetScale() const { return scale; }

public:

	std::vector<Weight> weights;
	unsigned int numWeights;

	unsigned int boneMeshUId;

	float4x4 offset;
	float* position;
	float* rotation;
	float* scale;
};