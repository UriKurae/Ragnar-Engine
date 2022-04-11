#pragma once
#include "Resource.h"
#include "Math/float4x4.h"
#include "Math/Quat.h"

struct BoneInfo
{
	int id;
	float4x4 offset;
};

struct Keys
{
	float4x4 matrix = {};
	float timeStamp = 0.0f;
};

struct BoneData
{
	std::vector<Keys> keyFrames = {};

	std::string name = "";
	int parentId = -1;
	int id = 0;
};

class Bone
{
public:
	Bone() {}
	Bone(BoneData boneData, int Id = 0);
	~Bone();

	void Update(float animationTime);
	void UpdateInterpolation(Bone& bone, float animationTime, float lastAnimTime, bool& interpolating, float velocity);

	BoneData& GetData() { return data; }

	inline const std::string& GetName() const { return data.name; }
	const int GetParentId() const { return parentId; }

	inline math::float4x4 GetTransform() { return localTransform; }

	int GetPositionIndex(float animationTime);
	int GetRotationIndex(float animationTime);
	int GetScalingIndex(float animationTime);
private:
	// Interpolations for normal animations
	float4x4 InterpolatePosition(float animationTime);
	float4x4 InterpolateRotation(float animationTime);
	float4x4 InterpolateScaling(float animationTime);

	// Interpolations for between animations
	float4x4 InterpolatePosition(Bone& bone, float animationTime, float lastAnimTime, bool& interpolating, float velocity);
	float4x4 InterpolateRotation(Bone& bone, float animationTime, float lastAnimTime, bool& interpolating, float velocity);
	float4x4 InterpolateScaling(Bone& bone, float animationTime, float lastAnimTime, bool& interpolating, float velocity);


	float GetScaleFactor(float lastTimeStamp, float nextTimeStamp, float animationTime);

private:
	BoneData data;
	int parentId;
	math::float4x4 localTransform;
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