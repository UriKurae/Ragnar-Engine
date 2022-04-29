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
	int id = 0;
};

class Bone
{
public:
	Bone(){}
	Bone(BoneData boneData);
	~Bone();

	void Update(float animationTime);
	void UpdateInterpolation(Bone& bone, float animationTime, float lastAnimTime, bool& interpolating, float velocity);

	BoneData& GetData() { return data; }

	inline const std::string& GetName() const { return data.name; }

	inline const float4x4& GetTransform() { return localTransform; }

	int GetIndex(float animationTime);
private:
	// Interpolations for normal animations
	const float4x4& Interpolate(float animationTime);

	// Interpolations for between animations
	float4x4 InterpolateBetweenLastAnim(Bone& bone, float animationTime, float lastAnimTime, bool& interpolating, float velocity);


	float GetScaleFactor(float lastTimeStamp, float nextTimeStamp, float animationTime);

private:
	BoneData data;

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