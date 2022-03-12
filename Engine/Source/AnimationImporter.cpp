#include "AnimationImporter.h"

#include "ResourceManager.h"
#include <vector>
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
		int boneKeys = animation->mChannels[i]->mNumPositionKeys;
		for (int j = 0; j < boneKeys; ++j)
		{
			KeyPosition pos;
			pos.position.x = animation->mChannels[i]->mPositionKeys[j].mValue.x;
			pos.position.y = animation->mChannels[i]->mPositionKeys[j].mValue.y;
			pos.position.z = animation->mChannels[i]->mPositionKeys[j].mValue.z;
			pos.timeStamp = animation->mChannels[i]->mPositionKeys[j].mTime;
			bone.positions.push_back(pos);
		}

		// Save Key scales
		boneKeys = animation->mChannels[i]->mNumScalingKeys;
		for (int j = 0; j < boneKeys; ++j)
		{
			KeyScale scale;
			scale.scale.x = animation->mChannels[i]->mScalingKeys[j].mValue.x;
			scale.scale.y = animation->mChannels[i]->mScalingKeys[j].mValue.y;
			scale.scale.z = animation->mChannels[i]->mScalingKeys[j].mValue.z;
			scale.timeStamp = animation->mChannels[i]->mScalingKeys[j].mTime;
			bone.scales.push_back(scale);
		}

		// Save Key Rotations
		boneKeys = animation->mChannels[i]->mNumRotationKeys;
		for (int j = 0; j < boneKeys; ++j)
		{
			KeyRotation rotations;
			rotations.orientation.x = animation->mChannels[i]->mRotationKeys[j].mValue.x;
			rotations.orientation.y = animation->mChannels[i]->mRotationKeys[j].mValue.y;
			rotations.orientation.z = animation->mChannels[i]->mRotationKeys[j].mValue.z;
			rotations.orientation.w = animation->mChannels[i]->mRotationKeys[j].mValue.w;
			rotations.timeStamp = animation->mChannels[i]->mRotationKeys[j].mTime;
			bone.rotations.push_back(rotations);
		}

		bones.push_back(bone);
	}

	HierarchyData data;
	ReadHierarchyData(data, scene->mRootNode, bones, boneCount);

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

	unsigned int header[4] = { duration, ticksPerSecond, boneData.size(), jsonName.size()};

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

		// Prepare array for positions
		itemName = "Positions" + std::to_string(i);
		JSON_Array* positionsArray = bone.SetNewJsonArray(bone.GetRootValue(), itemName.c_str());
		int size = boneData[i].positions.size();
		for (int j = 0; j < size; ++j)
		{
			// Save all positions
			json_array_append_number(positionsArray, boneData[i].positions[j].position.x);
			json_array_append_number(positionsArray, boneData[i].positions[j].position.y);
			json_array_append_number(positionsArray, boneData[i].positions[j].position.z);
			json_array_append_number(positionsArray, boneData[i].positions[j].timeStamp);
		}

		// Prepare array for scales
		itemName = "Scales" + std::to_string(i);
		JSON_Array* scalesArray = bone.SetNewJsonArray(bone.GetRootValue(), itemName.c_str());
		size = boneData[i].scales.size();
		for (int j = 0; j < size; ++j)
		{
			// Save all Scales
			json_array_append_number(scalesArray, boneData[i].scales[j].scale.x);
			json_array_append_number(scalesArray, boneData[i].scales[j].scale.y);
			json_array_append_number(scalesArray, boneData[i].scales[j].scale.z);
			json_array_append_number(scalesArray, boneData[i].scales[j].timeStamp);
		}

		// Prepare array for rotations
		itemName = "Rotations" + std::to_string(i);
		JSON_Array* rotationsArray = bone.SetNewJsonArray(bone.GetRootValue(), itemName.c_str());
		size = boneData[i].rotations.size();
		for (int j = 0; j < size; ++j)
		{
			// Save all rotations
			json_array_append_number(rotationsArray, boneData[i].rotations[j].orientation.x);
			json_array_append_number(rotationsArray, boneData[i].rotations[j].orientation.y);
			json_array_append_number(rotationsArray, boneData[i].rotations[j].orientation.z);
			json_array_append_number(rotationsArray, boneData[i].rotations[j].orientation.w);
			json_array_append_number(rotationsArray, boneData[i].rotations[j].timeStamp);
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

			aux = "Positions" + std::to_string(i);
			JSON_Array* positionsArray = jsonBone.GetJsonArray(jsonBone.ValueToObject(jsonBone.GetRootValue()), aux.c_str());
			size_t size = file.GetJsonArrayCount(positionsArray);
			for (int j = 0; j < size; j += 4)
			{
				KeyPosition pos;
				pos.position.x = json_array_get_number(positionsArray, j);
				pos.position.y = json_array_get_number(positionsArray, j + 1);
				pos.position.z = json_array_get_number(positionsArray, j + 2);
				pos.timeStamp = json_array_get_number(positionsArray, j + 3);

				boneData.positions.push_back(pos);
			}

			aux = "Rotations" + std::to_string(i);
			positionsArray = jsonBone.GetJsonArray(jsonBone.ValueToObject(jsonBone.GetRootValue()), aux.c_str());
			size = file.GetJsonArrayCount(positionsArray);
			for (int j = 0; j < size; j += 5)
			{
				KeyRotation rot;
				rot.orientation.x = json_array_get_number(positionsArray, j);
				rot.orientation.y = json_array_get_number(positionsArray, j + 1);
				rot.orientation.z = json_array_get_number(positionsArray, j + 2);
				rot.orientation.w = json_array_get_number(positionsArray, j + 3);
				rot.timeStamp = json_array_get_number(positionsArray, j + 4);

				boneData.rotations.push_back(rot);
			}

			aux = "Scales" + std::to_string(i);
			positionsArray = jsonBone.GetJsonArray(jsonBone.ValueToObject(jsonBone.GetRootValue()), aux.c_str());
			size = file.GetJsonArrayCount(positionsArray);
			for (int j = 0; j < size; j += 4)
			{
				KeyScale sca;
				sca.scale.x = json_array_get_number(positionsArray, j);
				sca.scale.y = json_array_get_number(positionsArray, j + 1);
				sca.scale.z = json_array_get_number(positionsArray, j + 2);
				sca.timeStamp = json_array_get_number(positionsArray, j + 3);

				boneData.scales.push_back(sca);
			}

			Bone bone(boneData);
			boneVector.push_back(bone);
		}
		jsonBone = file.GetJsonArrayValue(jsonArray, 1);
		LoadHierarchyData(hierData, jsonBone);
	}

	RELEASE_ARRAY(buffer);
}

void AnimationImporter::ReadHierarchyData(HierarchyData& data, aiNode* node, std::vector<BoneData>& bones, int count)
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

		KeyPosition position;
		position.position = pos;
		position.timeStamp = 0.0f;

		boneData.positions.push_back(position);

		KeyRotation rotation;
		rotation.orientation = rot;
		rotation.timeStamp = 0.0f;

		boneData.rotations.push_back(rotation);

		KeyScale scale;
		scale.scale = sca;
		scale.timeStamp = 0.0f;

		boneData.scales.push_back(scale);

		bones.push_back(boneData);
	}

	for (int i = 0; i < data.childrenCount; ++i)
	{
		HierarchyData childData;
		ReadHierarchyData(childData, node->mChildren[i], bones, count);
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
