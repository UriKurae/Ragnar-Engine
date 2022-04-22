#include "AnimationImporter.h"

#include "ResourceManager.h"
#include <vector>
#include <stack>
#include "Globals.h"
#include "FileSystem.h"
#include "Application.h"
#include "ResourceManager.h"
#include "Component.h"

#include "Profiling.h"

void AnimationImporter::ImportAnimations2(std::string& path, const aiScene* scene, JsonParsing& json, std::vector<uint>& uids, std::map<std::string, BoneInfo>& bones)
{
	for (unsigned int i = 0; i < scene->mNumAnimations; i++)
	{
		ImportAnimation2(path, scene, scene->mAnimations[i], json, uids, bones);
	}
}

void AnimationImporter::ImportAnimation2(std::string& path, const aiScene* scene, const aiAnimation* animation, JsonParsing& json, std::vector<uint>& uids, std::map<std::string, BoneInfo>& bonesID)
{
	int duration = animation->mDuration;
	int ticksPerSecond = animation->mTicksPerSecond;
	std::vector<BoneData> bones;

	int boneCount = 0;
	for (int i = 0; i < animation->mNumChannels; ++i)
	{
		BoneData bone;

		std::string boneName = animation->mChannels[i]->mNodeName.data;
		bone.name = boneName;

		if (bonesID.find(boneName) == bonesID.end())
		{
			bone.id = boneCount;
			boneCount++;
		}
		else
		{
			bone.id = bonesID[boneName].id;
		}

		// Save Key positions
		int keyPositions = animation->mChannels[i]->mNumPositionKeys;
		int keyScales = animation->mChannels[i]->mNumScalingKeys;
		int keyRotations = animation->mChannels[i]->mNumRotationKeys;
		int size = keyRotations;

		if (size < keyScales)
		{
			size = keyScales;
		}
		if (size < keyPositions)
		{
			size = keyPositions;
		}

		Keys keyframes;
		for (int j = 0, k = 0, l = 0; j < size && k < size && l < size; ++j, ++k, ++l)
		{

			float3 pos;
			if (j == keyPositions) j = keyPositions - 1;
			pos.x = animation->mChannels[i]->mPositionKeys[j].mValue.x;
			pos.y = animation->mChannels[i]->mPositionKeys[j].mValue.y;
			pos.z = animation->mChannels[i]->mPositionKeys[j].mValue.z;

			float3 scales;
			if (k == keyScales) k = keyScales - 1;
			/*scales.x = animation->mChannels[i]->mScalingKeys[k].mValue.x;
			scales.y = animation->mChannels[i]->mScalingKeys[k].mValue.y;
			scales.z = animation->mChannels[i]->mScalingKeys[k].mValue.z;*/
			scales.x = 1.0f;
			scales.y = 1.0f;
			scales.z = 1.0f;

			Quat rot;
			if (l == keyRotations) l = keyRotations - 1;
			rot.x = animation->mChannels[i]->mRotationKeys[l].mValue.x;
			rot.y = animation->mChannels[i]->mRotationKeys[l].mValue.y;
			rot.z = animation->mChannels[i]->mRotationKeys[l].mValue.z;
			rot.w = animation->mChannels[i]->mRotationKeys[l].mValue.w;

			keyframes.matrix = keyframes.matrix.FromTRS(pos, rot, scales);
			keyframes.timeStamp = animation->mChannels[i]->mRotationKeys[l].mTime;

			if (size < keyScales)
			{
				keyframes.timeStamp = animation->mChannels[i]->mScalingKeys[k].mTime;
			}
			if (size < keyPositions)
			{
				keyframes.timeStamp = animation->mChannels[i]->mPositionKeys[j].mTime;
			}

			bone.keyFrames.push_back(keyframes);
		}

		bones.push_back(bone);
	}

	HierarchyData data;
	ReadHierarchyData(data, scene->mRootNode, bones, boneCount);

	FilterBones(bones);

	for (int i = 0; i < bones.size(); i++)
	{
		if (bones[i].name.find("$") != std::string::npos)
		{
			bones.erase(bones.begin() + i);
			i = 0;
		}
	}

	std::vector<int> usedIds;
	for (int i = 0; i < bones.size(); i++)
	{
		if (bonesID.find(bones[i].name) != bonesID.end())
		{
			bones[i].id = bonesID[bones[i].name].id;
			usedIds.push_back(bones[i].id);
		}
		else
		{
			bones[i].id = -1;
		}
	}
	int index = 0;
	for (int i = 0; i < bones.size(); ++i)
	{
		if (bones[i].id == -1)
		{
			while (std::find(usedIds.begin(), usedIds.end(), index) != usedIds.end())
			{
				++index;
			}
			bones[i].id = index;
			usedIds.push_back(index);
		}
	}

	ParentBones(bones, data);

	std::string animName;
	std::string assetsPath(path);
	std::string name("__");
	name += animation->mName.C_Str();

	assetsPath.insert(assetsPath.find_last_of("."), name.c_str());

	uint uid = ResourceManager::GetInstance()->CreateResource(ResourceType::ANIMATION, assetsPath, animName);

	uids.push_back(uid);

	SaveAnimation2(animName, duration, ticksPerSecond, bones, data);

	JSON_Array* array = json.SetNewJsonArray(json.GetRootValue(), "Components");
	JsonParsing parse = JsonParsing();
	parse.SetNewJsonNumber(parse.ValueToObject(parse.GetRootValue()), "Type", (int)ComponentType::ANIMATION);
	parse.SetNewJsonString(parse.ValueToObject(parse.GetRootValue()), "Animation Path", animName.c_str());

	json.SetValueToArray(array, parse.GetRootValue());

}

void AnimationImporter::SaveAnimation2(std::string& name, float duration, float ticksPerSecond, std::vector<BoneData>& boneData, HierarchyData& hierData)
{
	// Json file to Save
	JsonParsing json = JsonParsing();

	std::string jsonName = name;

	int index = jsonName.find(".");
	jsonName = jsonName.substr(0, index);
	jsonName += ".json";

	unsigned int header[4] = { duration, ticksPerSecond, boneData.size(), jsonName.size() };

	uint size = sizeof(header) + sizeof(BoneData) * boneData.size() + jsonName.size();

	char* buffer = new char[size];
	char* cursor = buffer;

	unsigned int bytes = sizeof(header);

	// Save header
	memcpy(cursor, header, bytes);
	cursor += bytes;

	// First Save bonedata into a Json array
	JSON_Array* array = json.SetNewJsonArray(json.GetRootValue(), "Bones");
	JsonParsing bone = JsonParsing();
	for (int i = 0; i < boneData.size(); ++i)
	{
		// Id and Name values
		std::string itemName = "Id" + std::to_string(i);
		bone.SetNewJsonNumber(bone.ValueToObject(bone.GetRootValue()), itemName.c_str(), boneData[i].id);
		itemName = "Name" + std::to_string(i);
		bone.SetNewJsonString(bone.ValueToObject(bone.GetRootValue()), itemName.c_str(), boneData[i].name.c_str());

		int sizeKeys = boneData[i].keyFrames.size();
		itemName = "KeyFrameSize" + std::to_string(i);
		bone.SetNewJsonNumber(bone.ValueToObject(bone.GetRootValue()), itemName.c_str(), sizeKeys);

		// Prepare array for positions
		itemName = "Positions" + std::to_string(i);
		JSON_Array* positionsArray = bone.SetNewJsonArray(bone.GetRootValue(), itemName.c_str());

		std::string scalesName = "Scales" + std::to_string(i);
		JSON_Array* scalesArray = bone.SetNewJsonArray(bone.GetRootValue(), scalesName.c_str());

		std::string rotationName = "Rotations" + std::to_string(i);
		JSON_Array* rotationsArray = bone.SetNewJsonArray(bone.GetRootValue(), rotationName.c_str());

		std::string parentIdName = "ParentId" + std::to_string(i);
		bone.SetNewJsonNumber(bone.ValueToObject(bone.GetRootValue()), parentIdName.c_str(), boneData[i].parentId);

		for (int j = 0; j < sizeKeys; ++j)
		{
			float3 pos;
			float3 scales;
			Quat rot;
			boneData[i].keyFrames[j].matrix.Decompose(pos, rot, scales);

			// Save all positions
			json_array_append_number(positionsArray, pos.x);
			json_array_append_number(positionsArray, pos.y);
			json_array_append_number(positionsArray, pos.z);
			json_array_append_number(positionsArray, boneData[i].keyFrames[j].timeStamp);

			// Save all scales
			json_array_append_number(scalesArray, scales.x);
			json_array_append_number(scalesArray, scales.y);
			json_array_append_number(scalesArray, scales.z);
			json_array_append_number(scalesArray, boneData[i].keyFrames[j].timeStamp);

			// Save all rotations
			json_array_append_number(rotationsArray, rot.x);
			json_array_append_number(rotationsArray, rot.y);
			json_array_append_number(rotationsArray, rot.z);
			json_array_append_number(rotationsArray, rot.w);
			json_array_append_number(rotationsArray, boneData[i].keyFrames[j].timeStamp);


		}
	}
	// Append everything to the first json array
	json.SetValueToArray(array, bone.GetRootValue());

	// Old saving method for bone data using memcpy
	// Save BoneData vector
	//bytes = sizeof(BoneData) * boneData.size();
	//memcpy(cursor, boneData.data(), bytes);
	//cursor += bytes;

	const char* str = jsonName.c_str();
	bytes = strlen(str);
	memcpy(cursor, &str[0], bytes);
	cursor += bytes;

	JsonParsing hierJson = JsonParsing();
	SaveHierarchyData(hierData, hierJson);

	json.SetValueToArray(array, hierJson.GetRootValue());

	int jsonSize = json.SaveFile(jsonName.c_str());

	if (app->fs->Save(name.c_str(), buffer, size) > 0)
		DEBUG_LOG("Animation %s saved succesfully", name.c_str());

	RELEASE_ARRAY(buffer);

}

void AnimationImporter::LoadAnimation2(const char* path, float& ticks, float& ticksPerSecond, std::vector<Bone>& boneVector, HierarchyData& hierData)
{
	// WARNING: Uncommenting this causes to crash
	char* buffer = nullptr;
	app->fs->Load(path, &buffer);
	char* cursor = buffer;

	uint ranges[4];
	uint bytes = sizeof(ranges);
	memcpy(ranges, cursor, bytes);
	cursor += bytes;

	ticks = ranges[0];
	ticksPerSecond = ranges[1];

	int stringSize = ranges[3];

	std::string aux;
	aux.resize(stringSize);
	bytes = stringSize;
	memcpy(&aux[0], cursor, bytes);
	cursor += bytes;

	JsonParsing file = JsonParsing();
	file.ParseFile(aux.c_str());

	if (ranges[2] > 0)
	{
		// Old load method with memcpy for bones
		/*bytes = sizeof(BoneData);
		memcpy(&bone->GetData(), cursor, bytes);
		cursor += bytes;*/

		// Load bones
		JSON_Array* jsonArray = file.GetJsonArray(file.ValueToObject(file.GetRootValue()), "Bones");

		JsonParsing jsonBone = file.GetJsonArrayValue(jsonArray, 0);
		std::string aux;
		for (int i = 0; i < ranges[2]; ++i)
		{
			BoneData boneData;
			aux = "Id" + std::to_string(i);
			boneData.id = jsonBone.GetJsonNumber(aux.c_str());
			aux = "Name" + std::to_string(i);
			boneData.name = jsonBone.GetJsonString(aux.c_str());

			std::string parentIdName = "ParentId" + std::to_string(i);
			boneData.parentId = jsonBone.GetJsonNumber(parentIdName.c_str());

			aux = "Positions" + std::to_string(i);
			JSON_Array* positionsArray = jsonBone.GetJsonArray(jsonBone.ValueToObject(jsonBone.GetRootValue()), aux.c_str());
			size_t positionsSize = file.GetJsonArrayCount(positionsArray);

			std::string scales = "Scales" + std::to_string(i);
			JSON_Array* scalesArray = jsonBone.GetJsonArray(jsonBone.ValueToObject(jsonBone.GetRootValue()), scales.c_str());
			size_t scalesSize = file.GetJsonArrayCount(scalesArray);

			std::string rotations = "Rotations" + std::to_string(i);
			JSON_Array* rotationsArray = jsonBone.GetJsonArray(jsonBone.ValueToObject(jsonBone.GetRootValue()), rotations.c_str());
			size_t rotationsSize = file.GetJsonArrayCount(rotationsArray);

			std::string sizeKeyFrames = "KeyFrameSize" + std::to_string(i);
			int size = jsonBone.GetJsonNumber(sizeKeyFrames.c_str());

			int indexPos = 0;
			int indexScales = 0;
			int indexRot = 0;
			for (int j = 0; j < size; ++j)
			{
				Keys keys;
				float3 positions;
				positions.x = json_array_get_number(positionsArray, indexPos);
				positions.y = json_array_get_number(positionsArray, indexPos + 1);
				positions.z = json_array_get_number(positionsArray, indexPos + 2);
				float timeStamp = json_array_get_number(positionsArray, indexPos + 3);
				indexPos += 4;

				float3 scales;
				scales.x = json_array_get_number(scalesArray, indexScales);
				scales.y = json_array_get_number(scalesArray, indexScales + 1);
				scales.z = json_array_get_number(scalesArray, indexScales + 2);
				timeStamp = json_array_get_number(scalesArray, indexScales + 3);
				indexScales += 4;

				Quat rot;
				rot.x = json_array_get_number(rotationsArray, indexRot);
				rot.y = json_array_get_number(rotationsArray, indexRot + 1);
				rot.z = json_array_get_number(rotationsArray, indexRot + 2);
				rot.w = json_array_get_number(rotationsArray, indexRot + 3);
				timeStamp = json_array_get_number(rotationsArray, indexRot + 4);
				indexRot += 5;

				keys.matrix = keys.matrix.FromTRS(positions, rot, scales);
				keys.timeStamp = timeStamp;
				boneData.keyFrames.push_back(keys);
			}

			Bone bone(boneData, boneData.parentId);
			boneVector.push_back(bone);
		}
		jsonBone = file.GetJsonArrayValue(jsonArray, 1);
		LoadHierarchyData(hierData, jsonBone);
	}

	RELEASE_ARRAY(buffer);
}

void AnimationImporter::ReadHierarchyData(HierarchyData& data, aiNode* node, std::vector<BoneData>& bones, int count, int index)
{
	data.name = node->mName.C_Str();

	aiVector3D position;
	aiQuaternion rotation;
	aiVector3D scale;
	node->mTransformation.Decompose(scale, rotation, position);

	float3 pos = { position.x, position.y, position.z };
	Quat rot = { rotation.x, rotation.y, rotation.z, rotation.w };
	float3 sca = { scale.x, scale.y, scale.z };

	data.transform = float4x4::FromTRS(pos, rot, sca);
	data.childrenCount = node->mNumChildren;

	bool find = false;
	for (int i = 0; i < bones.size(); ++i)
	{
		if (bones[i].name == data.name)
		{
			find = true;
			break;
		}
	}

	if (!find)
	{
		BoneData boneData;
		boneData.id = count;
		boneData.name = data.name;

		Keys keyframe;
		keyframe.matrix = data.transform;
		keyframe.timeStamp = 0.0f;
		boneData.keyFrames.push_back(keyframe);

		bones.insert(bones.begin() + index, boneData);
	}
	else
	{
		for (int i = 0; i < bones.size(); ++i)
		{
			if (bones[i].name == data.name)
			{
				BoneData d = bones[i];
				bones.erase(bones.begin() + i);
				bones.insert(bones.begin() + index, d);
				break;
			}
		}
	}

	index++;

	for (int i = 0; i < data.childrenCount; ++i)
	{
		HierarchyData childData;
		ReadHierarchyData(childData, node->mChildren[i], bones, count, index);
		data.children.push_back(childData);
	}
}

void AnimationImporter::SaveHierarchyData(HierarchyData& data, JsonParsing& node)
{
	float3 position;
	Quat rotation;
	float3 scale;
	data.transform.Decompose(position, rotation, scale);

	node.SetNewJson3Number(node, "Position", position);
	node.SetNewJson4Number(node, "Rotation", rotation);
	node.SetNewJson3Number(node, "Scale", scale);

	node.SetNewJsonString(node.ValueToObject(node.GetRootValue()), "Name", data.name.c_str());
	node.SetNewJsonNumber(node.ValueToObject(node.GetRootValue()), "ChildrenCount", data.childrenCount);

	JSON_Array* array = node.SetNewJsonArray(node.GetRootValue(), "Children");
	for (int i = 0; i < data.childrenCount; ++i)
	{
		JsonParsing childrenJson = JsonParsing();
		SaveHierarchyData(data.children[i], childrenJson);
		node.SetValueToArray(array, childrenJson.GetRootValue());
	}
}

void AnimationImporter::LoadHierarchyData(HierarchyData& data, JsonParsing& node)
{
	float3 position = node.GetJson3Number(node, "Position");
	math::float4 auxRotation = node.GetJson4Number(node, "Rotation");
	Quat rotation = { auxRotation.x, auxRotation.y, auxRotation.z, auxRotation.w };
	float3 scale = node.GetJson3Number(node, "Scale");

	data.transform = math::float4x4::FromTRS(position, rotation, scale);
	data.name = node.GetJsonString("Name");
	data.childrenCount = node.GetJsonNumber("ChildrenCount");
	JSON_Array* array = node.GetJsonArray(node.ValueToObject(node.GetRootValue()), "Children");

	data.children.resize(data.childrenCount);
	for (int i = 0; i < data.childrenCount; ++i)
	{
		JsonParsing children = node.GetJsonArrayValue(array, i);
		LoadHierarchyData(data.children[i], children);
	}
}

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

	RELEASE_ARRAY(boneTransformations);
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

void AnimationImporter::LoadAnimation(const char* path, float& ticks, float& ticksPerSecond, int& numBones, BoneTransform** boneTransformations)
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
		*boneTransformations = new BoneTransform[numBones];

		//Loading Bones
		for (int i = 0; i < numBones; i++)
		{
			//Loading Ranges
			uint boneRanges[4];
			uint bytes = sizeof(boneRanges);
			memcpy(boneRanges, cursor, bytes);
			cursor += bytes;

			(*boneTransformations)[i].numPosKeys = boneRanges[0];
			(*boneTransformations)[i].numScaleKeys = boneRanges[1];
			(*boneTransformations)[i].numRotKeys = boneRanges[2];

			//Loading Name
			bytes = boneRanges[3];
			char* auxName = new char[bytes];
			memcpy(auxName, cursor, bytes);
			(*boneTransformations)[i].nodeName = auxName;
			(*boneTransformations)[i].nodeName = (*boneTransformations)[i].nodeName.substr(0, bytes);
			RELEASE_ARRAY(auxName);
			cursor += bytes;

			//Loading Pos Time
			bytes = (*boneTransformations)[i].numPosKeys * sizeof(double);
			(*boneTransformations)[i].posKeysTimes = new double[(*boneTransformations)[i].numPosKeys];
			memcpy((*boneTransformations)[i].posKeysTimes, cursor, bytes);
			cursor += bytes;
			//Loading Pos Values
			bytes = (*boneTransformations)[i].numPosKeys * sizeof(float) * 3;
			(*boneTransformations)[i].posKeysValues = new float[(*boneTransformations)[i].numPosKeys * 3];
			memcpy((*boneTransformations)[i].posKeysValues, cursor, bytes);
			cursor += bytes;

			//Loading Scale Time
			bytes = (*boneTransformations)[i].numScaleKeys * sizeof(double);
			(*boneTransformations)[i].scaleKeysTimes = new double[(*boneTransformations)[i].numScaleKeys];
			memcpy((*boneTransformations)[i].scaleKeysTimes, cursor, bytes);
			cursor += bytes;
			//Loading Scale Values
			bytes = (*boneTransformations)[i].numScaleKeys * sizeof(float) * 3;
			(*boneTransformations)[i].scaleKeysValues = new float[(*boneTransformations)[i].numScaleKeys * 3];
			memcpy((*boneTransformations)[i].scaleKeysValues, cursor, bytes);
			cursor += bytes;

			//Loading Rotation Time
			bytes = (*boneTransformations)[i].numRotKeys * sizeof(double);
			(*boneTransformations)[i].rotKeysTimes = new double[(*boneTransformations)[i].numRotKeys];
			memcpy((*boneTransformations)[i].rotKeysTimes, cursor, bytes);
			cursor += bytes;
			//Loading Rotation Values
			bytes = (*boneTransformations)[i].numRotKeys * sizeof(float) * 4;
			(*boneTransformations)[i].rotKeysValues = new float[(*boneTransformations)[i].numRotKeys * 4];
			memcpy((*boneTransformations)[i].rotKeysValues, cursor, bytes);
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

void AnimationImporter::SaveBone(std::string& name, unsigned int numWeights, float* pos, float* rot, float* scale, std::vector<Weight>& weights)
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

void AnimationImporter::FilterBones(std::vector<BoneData>& bones)
{
	std::vector<BoneData> dollarsVisited;
	for (int i = 0; i < bones.size(); ++i)
	{
		if (bones[i].name.find("$") != std::string::npos)
		{
			dollarsVisited.push_back(bones[i]);
		}
		else
		{
			if (!dollarsVisited.empty())
			{
				for (int j = dollarsVisited.size() - 1; 0 <= j; --j)
				{
					int size = dollarsVisited[j].keyFrames.size();
					if (bones[i].keyFrames.size() < size)
					{
						for (int l = bones[i].keyFrames.size() - 1; l < size - 1; ++l)
						{
							bones[i].keyFrames.push_back(bones[i].keyFrames[l]);
						}
					}

					int k = 0;
					int index = 0;
					for (k = 0; k < size; k++)
					{
						bones[i].keyFrames[k].matrix = bones[i].keyFrames[k].matrix * dollarsVisited[j].keyFrames[k].matrix;
						bones[i].keyFrames[k].timeStamp = dollarsVisited[j].keyFrames[k].timeStamp;

						index = k;
					}
					if (bones[i].keyFrames.size() > k)
					{
						float4x4 mat = bones[i].keyFrames[index].matrix;
						for (; k < bones[i].keyFrames.size(); ++k)
						{
							bones[i].keyFrames[k].matrix = mat;
						}
					}
				}
				dollarsVisited.clear();
			}
		}
	}
}

void AnimationImporter::ParentBones(std::vector<BoneData>& bones, HierarchyData& data)
{
	std::stack<HierarchyData> dataStack;

	dataStack.push(data);

	while (!dataStack.empty())
	{

		HierarchyData parent = dataStack.top();
		dataStack.pop();

		int idParent = 0;
		// Find the Id from the parent in the bones vector
		for (int i = 0; i < bones.size(); ++i)
		{
			if (bones[i].name == parent.name) idParent = bones[i].id;
		}

		// Iterate through the childrens of the parent
		for (int i = 0; i < parent.childrenCount; ++i)
		{
			for (int j = 0; j < bones.size(); ++j)
			{
				// If we find the bone of the children in the bones vector, we can put his parent's ID
				if (parent.children[i].name == bones[j].name)
				{
					bones[j].parentId = idParent;
				}
			}
			dataStack.push(parent.children[i]);
		}
	}
}
