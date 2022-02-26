#include "AnimationImporter.h"

#include "ResourceManager.h"
#include <vector>
#include "Globals.h"
#include "FileSystem.h"
#include "Application.h"
#include "ResourceManager.h"
#include "Component.h"

void AnimationImporter::ImportAnimations(std::string& path, const aiScene* scene, JsonParsing& json, std::vector<uint>& uids)
{
	for (unsigned int i = 0; i < scene->mNumAnimations; i++)
	{
		ImportAnimation(path, scene->mAnimations[i], json, uids);
	}
}

void AnimationImporter::ImportAnimation(std::string& path, const aiAnimation* animation, JsonParsing& json, std::vector<uint>& uids)
{
	float ticks = animation->mDuration;
	float ticksPerSecond = animation->mTicksPerSecond;
	int numBones = animation->mNumChannels;
	BoneTransform* boneTransformations = new BoneTransform[numBones];

	if (animation->mNumChannels > 0)
	{
		for (int i = 0; i < numBones; i++)
		{
			boneTransformations[i].nodeName = (animation->mChannels[i]->mNodeName.length > 0) ? animation->mChannels[i]->mNodeName.C_Str() : "Unnamed";
			boneTransformations[i].numPosKeys = animation->mChannels[i]->mNumPositionKeys;
			boneTransformations[i].posKeysValues = new float[boneTransformations[i].numPosKeys * 3];
			boneTransformations[i].posKeysTimes = new double[boneTransformations[i].numPosKeys];
			for (int j = 0; j < boneTransformations[i].numPosKeys; j++)
			{
				int value = j * 3;
				boneTransformations[i].posKeysTimes[j] = animation->mChannels[i]->mPositionKeys[j].mTime;
				boneTransformations[i].posKeysValues[value] = animation->mChannels[i]->mPositionKeys[j].mValue.x;
				boneTransformations[i].posKeysValues[value + 1] = animation->mChannels[i]->mPositionKeys[j].mValue.y;
				boneTransformations[i].posKeysValues[value + 2] = animation->mChannels[i]->mPositionKeys[j].mValue.z;
			}

			boneTransformations[i].numScaleKeys = animation->mChannels[i]->mNumScalingKeys;
			boneTransformations[i].scaleKeysValues = new float[boneTransformations[i].numScaleKeys * 3];
			boneTransformations[i].scaleKeysTimes = new double[boneTransformations[i].numScaleKeys];
			for (int j = 0; j < boneTransformations[i].numScaleKeys; j++)
			{
				int value = j * 3;
				boneTransformations[i].scaleKeysTimes[j] = animation->mChannels[i]->mScalingKeys[j].mTime;
				boneTransformations[i].scaleKeysValues[value] = animation->mChannels[i]->mScalingKeys[j].mValue.x;
				boneTransformations[i].scaleKeysValues[value + 1] = animation->mChannels[i]->mScalingKeys[j].mValue.y;
				boneTransformations[i].scaleKeysValues[value + 2] = animation->mChannels[i]->mScalingKeys[j].mValue.z;
			}

			boneTransformations[i].numRotKeys = animation->mChannels[i]->mNumRotationKeys;
			boneTransformations[i].rotKeysValues = new float[boneTransformations[i].numRotKeys * 4];
			boneTransformations[i].rotKeysTimes = new double[boneTransformations[i].numRotKeys];
			for (int j = 0; j < boneTransformations[i].numRotKeys; j++)
			{
				int value = j * 4;
				boneTransformations[i].rotKeysTimes[j] = animation->mChannels[i]->mRotationKeys[j].mTime;
				boneTransformations[i].rotKeysValues[value] = animation->mChannels[i]->mRotationKeys[j].mValue.x;
				boneTransformations[i].rotKeysValues[value + 1] = animation->mChannels[i]->mRotationKeys[j].mValue.y;
				boneTransformations[i].rotKeysValues[value + 2] = animation->mChannels[i]->mRotationKeys[j].mValue.z;
				boneTransformations[i].rotKeysValues[value + 3] = animation->mChannels[i]->mRotationKeys[j].mValue.w;
			}
		}
	}

	std::string animName;
	std::string assetsPath(path);
	std::string name("__");
	name += animation->mName.C_Str();

	assetsPath.insert(assetsPath.find_last_of("."), name.c_str());

	uint uid = ResourceManager::GetInstance()->CreateResource(ResourceType::ANIMATION, assetsPath, animName);

	uids.push_back(uid);

	SaveAnimation(animName, ticks, ticksPerSecond, numBones, boneTransformations);

	JSON_Array* array = json.SetNewJsonArray(json.GetRootValue(), "Components");
	JsonParsing parse = JsonParsing();
	parse.SetNewJsonNumber(parse.ValueToObject(parse.GetRootValue()), "Type", (int)ComponentType::ANIMATION);
	parse.SetNewJsonString(parse.ValueToObject(parse.GetRootValue()), "Animation Path", animName.c_str());

	json.SetValueToArray(array, parse.GetRootValue());
}

void AnimationImporter::CreateMetaAnimation(std::string& library, std::string& assets, uint uid)
{
	JsonParsing metaFile;

	metaFile.SetNewJsonString(metaFile.ValueToObject(metaFile.GetRootValue()), "Assets Path", assets.c_str());
	metaFile.SetNewJsonNumber(metaFile.ValueToObject(metaFile.GetRootValue()), "Uuid", uid);

	char* buffer = nullptr;
	size_t size = metaFile.Save(&buffer);

	app->fs->Save(library.c_str(), buffer, size);

	RELEASE_ARRAY(buffer);
}

void AnimationImporter::SaveAnimation(std::string& name, float ticks, float ticksPerSecond, int numBones, BoneTransform* boneTransformations)
{
	unsigned int header[3] = { ticks, ticksPerSecond, numBones };
	uint size = sizeof(header);

	for (int i = 0; i < numBones; i++)
	{
		unsigned int boneHeader[4] = { boneTransformations[i].numPosKeys, boneTransformations[i].numScaleKeys, boneTransformations[i].numRotKeys, boneTransformations[i].nodeName.size() };
		size += sizeof(boneHeader);
		size += boneTransformations[i].nodeName.size();

		size += sizeof(double) * boneTransformations[i].numPosKeys;
		size += sizeof(float) * boneTransformations[i].numPosKeys * 3;

		size += sizeof(double) * boneTransformations[i].numScaleKeys;
		size += sizeof(float) * boneTransformations[i].numScaleKeys * 3;

		size += sizeof(double) * boneTransformations[i].numRotKeys;
		size += sizeof(float) * boneTransformations[i].numRotKeys * 4;
	}

	char* buffer = new char[size];
	char* cursor = buffer;

	uint bytes = sizeof(header);
	memcpy(cursor, header, bytes);
	cursor += bytes;

	for (int i = 0; i < numBones; i++)
	{
		uint boneHeader[4] = { boneTransformations[i].numPosKeys, boneTransformations[i].numScaleKeys, boneTransformations[i].numRotKeys, boneTransformations[i].nodeName.size() };

		bytes = sizeof(boneHeader);
		memcpy(cursor, boneHeader, bytes);
		cursor += bytes;

		bytes = boneTransformations[i].nodeName.size();
		memcpy(cursor, boneTransformations[i].nodeName.c_str(), bytes);
		cursor += bytes;

		bytes = boneTransformations[i].numPosKeys * sizeof(double);
		memcpy(cursor, boneTransformations[i].posKeysTimes, bytes);
		cursor += bytes;
		bytes = boneTransformations[i].numPosKeys * sizeof(float) * 3;
		memcpy(cursor, boneTransformations[i].posKeysValues, bytes);
		cursor += bytes;

		bytes = boneTransformations[i].numScaleKeys * sizeof(double);
		memcpy(cursor, boneTransformations[i].scaleKeysTimes, bytes);
		cursor += bytes;
		bytes = boneTransformations[i].numScaleKeys * sizeof(float) * 3;
		memcpy(cursor, boneTransformations[i].scaleKeysValues, bytes);
		cursor += bytes;

		bytes = boneTransformations[i].numRotKeys * sizeof(double);
		memcpy(cursor, boneTransformations[i].rotKeysTimes, bytes);
		cursor += bytes;
		bytes = boneTransformations[i].numRotKeys * sizeof(float) * 4;
		memcpy(cursor, boneTransformations[i].rotKeysValues, bytes);
		cursor += bytes;
	}

	if (app->fs->Save(name.c_str(), buffer, size) > 0)
		DEBUG_LOG("Animation %s saved succesfully", name.c_str());

	RELEASE_ARRAY(buffer);
}

void AnimationImporter::LoadAnimation(const char* path, float& ticks, float& ticksPerSecond, int& numBones, BoneTransform* boneTransformations)
{

	char* buffer = nullptr;
	app->fs->Load(path, &buffer);
	char* cursor = buffer;

	uint ranges[3];
	uint bytes = sizeof(ranges);
	memcpy(ranges, cursor, bytes);
	cursor += bytes;

	ticks = ranges[0];
	ticksPerSecond = ranges[1];
	numBones = ranges[2];

	if (numBones > 0)
	{
		boneTransformations = new BoneTransform[numBones];

		//Loading Bones
		for (int i = 0; i < numBones; i++)
		{
			//Loading Ranges
			uint boneRanges[4];
			uint bytes = sizeof(boneRanges);
			memcpy(boneRanges, cursor, bytes);
			cursor += bytes;

			boneTransformations[i].numPosKeys = boneRanges[0];
			boneTransformations[i].numScaleKeys = boneRanges[1];
			boneTransformations[i].numRotKeys = boneRanges[2];

			//Loading Name
			bytes = boneRanges[3];
			char* auxName = new char[bytes];
			memcpy(auxName, cursor, bytes);
			boneTransformations[i].nodeName = auxName;
			boneTransformations[i].nodeName = boneTransformations[i].nodeName.substr(0, bytes);
			RELEASE_ARRAY(auxName);
			cursor += bytes;

			//Loading Pos Time
			bytes = boneTransformations[i].numPosKeys * sizeof(double);
			boneTransformations[i].posKeysTimes = new double[boneTransformations[i].numPosKeys];
			memcpy(boneTransformations[i].posKeysTimes, cursor, bytes);
			cursor += bytes;
			//Loading Pos Values
			bytes = boneTransformations[i].numPosKeys * sizeof(float) * 3;
			boneTransformations[i].posKeysValues = new float[boneTransformations[i].numPosKeys * 3];
			memcpy(boneTransformations[i].posKeysValues, cursor, bytes);
			cursor += bytes;

			//Loading Scale Time
			bytes = boneTransformations[i].numScaleKeys * sizeof(double);
			boneTransformations[i].scaleKeysTimes = new double[boneTransformations[i].numScaleKeys];
			memcpy(boneTransformations[i].scaleKeysTimes, cursor, bytes);
			cursor += bytes;
			//Loading Scale Values
			bytes = boneTransformations[i].numScaleKeys * sizeof(float) * 3;
			boneTransformations[i].scaleKeysValues = new float[boneTransformations[i].numScaleKeys * 3];
			memcpy(boneTransformations[i].scaleKeysValues, cursor, bytes);
			cursor += bytes;

			//Loading Rotation Time
			bytes = boneTransformations[i].numRotKeys * sizeof(double);
			boneTransformations[i].rotKeysTimes = new double[boneTransformations[i].numRotKeys];
			memcpy(boneTransformations[i].rotKeysTimes, cursor, bytes);
			cursor += bytes;
			//Loading Rotation Values
			bytes = boneTransformations[i].numRotKeys * sizeof(float) * 4;
			boneTransformations[i].rotKeysValues = new float[boneTransformations[i].numRotKeys * 4];
			memcpy(boneTransformations[i].rotKeysValues, cursor, bytes);
			cursor += bytes;
		}
	}

	RELEASE_ARRAY(buffer);
}


// Bones
void AnimationImporter::ReImportBones(std::string& path, aiBone* bone, JsonParsing& json, std::string& library, std::vector<unsigned int>& bonesUid)
{
	unsigned int numWeights;

	float* pos;
	float* rot;
	float* scale;

	aiVector3D translation;
	aiVector3D scaling;
	aiQuaternion rotation;

	numWeights = bone->mNumWeights;
	std::vector<Weight> weights;

	bone->mOffsetMatrix.Decompose(scaling, rotation, translation);

	pos = new float[3];
	pos[0] = translation.x;
	pos[1] = translation.y;
	pos[2] = translation.z;

	rot = new float[4];
	rot[1] = rotation.y;
	rot[2] = rotation.z;
	rot[3] = rotation.w;
	rot[0] = rotation.x;

	scale = new float[3];
	scale[0] = scaling.x;
	scale[1] = scaling.y;
	scale[2] = scaling.z;

	if (bone->mNumWeights > 0)
	{
		for (int i = 0; i < numWeights; i++)
		{
			weights.emplace_back(Weight(bone->mWeights[i].mVertexId, bone->mWeights[i].mWeight));
		}
	}

	std::string boneName;
	std::string assetsPath(path);
	std::string name("__");
	name += bone->mName.C_Str();

	SaveBone(boneName, numWeights, pos, rot, scale, weights);

	JSON_Array* array = json.SetNewJsonArray(json.GetRootValue(), "Components");
	JsonParsing parse = JsonParsing();
	parse.SetNewJsonNumber(parse.ValueToObject(parse.GetRootValue()), "Type", (int)ComponentType::BONE);
	parse.SetNewJsonString(parse.ValueToObject(parse.GetRootValue()), "Bone Path", boneName.c_str());

	json.SetValueToArray(array, parse.GetRootValue());
}

void AnimationImporter::ImportBones(std::string& path, aiBone* bone, JsonParsing& json, std::vector<uint>& uids, std::vector<unsigned int>& bonesUid)
{
	unsigned int numWeights;

	float* pos;
	float* rot;
	float* scale;

	aiVector3D translation;
	aiVector3D scaling;
	aiQuaternion rotation;

	numWeights = bone->mNumWeights;
	std::vector<Weight> weights;

	bone->mOffsetMatrix.Decompose(scaling, rotation, translation);

	pos = new float[3];
	pos[0] = translation.x;
	pos[1] = translation.y;
	pos[2] = translation.z;

	rot = new float[4];
	rot[1] = rotation.y;
	rot[2] = rotation.z;
	rot[3] = rotation.w;
	rot[0] = rotation.x;

	scale = new float[3];
	scale[0] = scaling.x;
	scale[1] = scaling.y;
	scale[2] = scaling.z;

	if (bone->mNumWeights > 0)
	{
		for (int i = 0; i < numWeights; i++)
		{
			weights.emplace_back(Weight(bone->mWeights[i].mVertexId, bone->mWeights[i].mWeight));
		}
	}

	std::string boneName;
	std::string assetsPath(path);
	std::string name("__");
	name += bone->mName.C_Str();

	assetsPath.insert(assetsPath.find_last_of("."), name.c_str());

	uint uid = ResourceManager::GetInstance()->CreateResource(ResourceType::BONE, assetsPath, boneName);

	uids.push_back(uid);
	bonesUid.push_back(uid);

	SaveBone(boneName, numWeights, pos, rot, scale, weights);

	JSON_Array* array = json.SetNewJsonArray(json.GetRootValue(), "Components");
	JsonParsing parse = JsonParsing();
	parse.SetNewJsonNumber(parse.ValueToObject(parse.GetRootValue()), "Type", (int)ComponentType::BONE);
	parse.SetNewJsonString(parse.ValueToObject(parse.GetRootValue()), "Bone Path", boneName.c_str());

	json.SetValueToArray(array, parse.GetRootValue());
}

void AnimationImporter::CreateMetaBones(std::string& library, std::string& assets, uint uid)
{
	JsonParsing metaFile;

	metaFile.SetNewJsonString(metaFile.ValueToObject(metaFile.GetRootValue()), "Assets Path", assets.c_str());
	metaFile.SetNewJsonNumber(metaFile.ValueToObject(metaFile.GetRootValue()), "Uuid", uid);

	char* buffer = nullptr;
	size_t size = metaFile.Save(&buffer);

	app->fs->Save(library.c_str(), buffer, size);

	RELEASE_ARRAY(buffer);
}

void AnimationImporter::SaveBone(std::string& name,unsigned int numWeights, float* pos, float* rot, float* scale, std::vector<Weight>& weights)
{
	uint header = numWeights;
	uint size = sizeof(header);

	size += sizeof(float) * 3;
	size += sizeof(float) * 4;
	size += sizeof(float) * 3;

	size += sizeof(uint) * numWeights;
	size += sizeof(float) * numWeights;

	char* buffer = new char[size];
	char* cursor = buffer;

	uint bytes = sizeof(header);
	memcpy(cursor, &header, bytes);
	cursor += bytes;

	bytes = sizeof(float) * 3;
	memcpy(cursor, pos, bytes);
	cursor += bytes;

	bytes = sizeof(float) * 4;
	memcpy(cursor, rot, bytes);
	cursor += bytes;

	bytes = sizeof(float) * 3;
	memcpy(cursor, scale, bytes);
	cursor += bytes;

	for (int i = 0; i < numWeights; i++)
	{
		bytes = sizeof(uint);
		memcpy(cursor, &weights[i].vertexId, bytes);
		cursor += bytes;

		bytes = sizeof(float);
		memcpy(cursor, &weights[i].weight, bytes);
		cursor += bytes;
	}

	if (app->fs->Save(name.c_str(), buffer, size) > 0)
		DEBUG_LOG("Bone %s saved succesfully", name.c_str());

	RELEASE_ARRAY(buffer);
}
