#include "Bone.h"
#include "Application.h"
#include "Globals.h"

#include "AnimationImporter.h"
#include "FileSystem.h"

#include "Math/TransformOps.h"

#include "Profiling.h"

Bone2::Bone2(uint uid, std::string& assets, std::string& library) : Resource(uid, ResourceType::BONE, assets, library)
{
	std::string metaPath = BONES_FOLDER + std::string("bone_") + std::to_string(uid) + ".meta";
	AnimationImporter::CreateMetaBones(metaPath, assetsPath, uid);
	name = assets;
	app->fs->GetFilenameWithoutExtension(name);
	name = name.substr(name.find_first_of("__") + 2, name.length());
}

Bone2::~Bone2()
{
	RELEASE_ARRAY(position);
	RELEASE_ARRAY(rotation);
	RELEASE_ARRAY(scale);
}

void Bone2::Load()
{
	//Get the buffer
	char* buffer = nullptr;
	app->fs->Load(libraryPath.c_str(), &buffer);
	char* cursor = buffer;

	uint bytes = sizeof(uint);
	memcpy(&numWeights, cursor, bytes);
	cursor += bytes;

	bytes = sizeof(float) * 3;
	position = new float[3];
	memcpy(position, cursor, bytes);
	cursor += bytes;

	bytes = sizeof(float) * 4;
	rotation = new float[4];
	memcpy(rotation, cursor, bytes);
	cursor += bytes;

	bytes = sizeof(float) * 3;
	scale = new float[3];
	memcpy(scale, cursor, bytes);
	cursor += bytes;

	offset = float4x4::FromTRS({ position[0],position[1],position[2] }, { rotation[0],rotation[1],rotation[2],rotation[3] }, { scale[0],scale[1],scale[2] });

	if (numWeights > 0)
	{
		uint v;
		float w;

		for (int i = 0; i < numWeights; i++)
		{
			bytes = sizeof(uint);
			memcpy(&v, cursor, bytes);
			cursor += bytes;

			bytes = sizeof(float);
			memcpy(&w, cursor, bytes);
			cursor += bytes;
		}

		weights.emplace_back(Weight(v, w));
	}

	RELEASE_ARRAY(buffer);
}

Bone::Bone(BoneData boneData) : data(boneData)
{
}

void Bone::Update(float animationTime)
{
	float4x4 translation = InterpolatePosition(animationTime);
	float4x4 rotation = InterpolateRotation(animationTime);
	float4x4 scale = InterpolateScaling(animationTime);
	localTransform = translation * rotation * scale;
}

void Bone::UpdateInterpolation(Bone& bone, float animationTime, float lastAnimTime, bool& interpolating, float velocity)
{
	float4x4 translation = InterpolatePosition(bone, animationTime, lastAnimTime, interpolating, velocity);
	float4x4 rotation = InterpolateRotation(bone, animationTime, lastAnimTime, interpolating, velocity);
	float4x4 scale = InterpolateScaling(bone, animationTime, lastAnimTime, interpolating, velocity);
	localTransform = translation * rotation * scale;
}

float4x4 Bone::InterpolatePosition(float animationTime)
{
	if (data.positions.size() == 1)
		return float4x4::Translate(data.positions[0].position);

	int p0Index = GetPositionIndex(animationTime);
	int p1Index = p0Index + 1;
	float scaleFactor = GetScaleFactor(data.positions[p0Index].timeStamp,
		data.positions[p1Index].timeStamp, animationTime);
	
	float3 finalPosition = math::Lerp(data.positions[p0Index].position, data.positions[p1Index].position, scaleFactor);
	
	return float4x4::Translate(finalPosition);
}

float4x4 Bone::InterpolateRotation(float animationTime)
{
	if (data.rotations.size() == 1)
	{
		Quat rotation = data.rotations[0].orientation.Normalized();
		return float4x4(rotation);
	}

	int p0Index = GetRotationIndex(animationTime);
	int p1Index = p0Index + 1;
	float scaleFactor = GetScaleFactor(data.rotations[p0Index].timeStamp, data.rotations[p1Index].timeStamp, animationTime);
	
	Quat finalRotation = Slerp(data.rotations[p0Index].orientation, data.rotations[p1Index].orientation, scaleFactor);
	finalRotation = finalRotation.Normalized();

	return float4x4(finalRotation);
}

float4x4 Bone::InterpolateScaling(float animationTime)
{
	if (data.scales.size() == 1)
		return float4x4::Scale(data.scales[0].scale);

	int p0Index = GetScalingIndex(animationTime);
	int p1Index = p0Index + 1;
	float scaleFactor = GetScaleFactor(data.scales[p0Index].timeStamp,
		data.scales[p1Index].timeStamp, animationTime);
	float3 finalScale = math::Lerp(data.scales[p0Index].scale, data.scales[p1Index].scale, scaleFactor);
	
	return float4x4::Scale(finalScale);
}

float4x4 Bone::InterpolatePosition(Bone& bone, float animationTime, float lastAnimTime, bool& interpolating, float velocity)
{
	int index = bone.GetPositionIndex(lastAnimTime);
	float scaleFactor = GetScaleFactor(0.0f, velocity, animationTime);

	float3 finalPosition = math::Lerp(bone.data.positions[index].position, data.positions[0].position, scaleFactor);

	if (scaleFactor >= 1.0f)
		interpolating = false;

	return float4x4::Translate(finalPosition);
}

float4x4 Bone::InterpolateRotation(Bone& bone, float animationTime, float lastAnimTime, bool& interpolating, float velocity)
{
	int index = bone.GetRotationIndex(lastAnimTime);
	float scaleFactor = GetScaleFactor(0.0f, velocity, animationTime);

	Quat finalRotation = math::Slerp(bone.data.rotations[index].orientation, data.rotations[0].orientation, scaleFactor);
	finalRotation = finalRotation.Normalized();

	if (scaleFactor >= 1.0f)
		interpolating = false;

	return float4x4(finalRotation);
}

float4x4 Bone::InterpolateScaling(Bone& bone, float animationTime, float lastAnimTime, bool& interpolating, float velocity)
{
	int index = bone.GetScalingIndex(lastAnimTime);
	float scaleFactor = GetScaleFactor(0, velocity, animationTime);

	float3 finalScale = math::Lerp(bone.data.scales[index].scale, data.scales[0].scale, scaleFactor);

	if (scaleFactor >= 1.0f)
		interpolating = false;

	return float4x4::Scale(finalScale);
}

int Bone::GetPositionIndex(float animationTime)
{
	for (int index = 0; index < data.positions.size(); ++index)
	{
		if (index + 1 < data.positions.size())
		{
			if (animationTime < data.positions[index + 1].timeStamp)
				return index;
		}
	}

	return 0;
}

int Bone::GetRotationIndex(float animationTime)
{
	for (int index = 0; index < data.rotations.size(); ++index)
	{
		if (index + 1 < data.rotations.size())
		{
			if (animationTime < data.rotations[index + 1].timeStamp)
				return index;
		}
	}

	return 0;
}

int Bone::GetScalingIndex(float animationTime)
{
	for (int index = 0; index < data.scales.size(); ++index)
	{
		if (index + 1 < data.scales.size())
		{
			if (animationTime < data.scales[index + 1].timeStamp)
				return index;
		}
	}

	return 0;
}

float Bone::GetScaleFactor(float lastTimeStamp, float nextTimeStamp, float animationTime)
{
	// WARNING: This here sometimes is a division by 0, so it causes inf (NaN)
	float scaleFactor = 0.0f;
	float midWayLength = animationTime - lastTimeStamp;
	float framesDiff = nextTimeStamp - lastTimeStamp;
	scaleFactor = midWayLength / framesDiff;
	if (scaleFactor < 0) return 0.0f;

	return scaleFactor;
}