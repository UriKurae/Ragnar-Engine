#pragma once

#include <string>
#include <map>

#include "assimp/anim.h"
#include "assimp/mesh.h"
#include "assimp/scene.h"
#include "Animation.h"
#include "Bone.h"

class JsonParsing;

namespace AnimationImporter
{
	void ImportAnimations2(std::string& path, const aiScene* scene, JsonParsing& json, std::vector<uint>& uids, std::map<std::string, BoneInfo>& bones);
	void ImportAnimation2(std::string& path, const aiScene* scene, const aiAnimation* animation, JsonParsing& json, std::vector<uint>& uids, std::map<std::string, BoneInfo>& bones);
	void SaveAnimation2(std::string& name, float duration, float ticksPerSecond, std::vector<BoneData>& boneData, HierarchyData& hierData);
	void LoadAnimation2(const char* path, float& ticks, float& ticksPerSecond, std::vector<Bone*> &boneVector, HierarchyData hierData);
	void ReadHierarchyData(HierarchyData& data, aiNode* node);
	void SaveHierarchyData(HierarchyData& data, JsonParsing& node);
	void LoadHierarchyData(HierarchyData& data, JsonParsing& node);

	void ImportAnimations(std::string& path, const aiScene* scene, JsonParsing& json, std::vector<uint>& uids);
	void ImportAnimation(std::string& path, const aiAnimation* animation, JsonParsing& json, std::vector<uint>& uids);
	void CreateMetaAnimation(std::string& library, std::string& assets, uint uid);
	void SaveAnimation(std::string& name, float ticks, float ticksPerSecond, int numBones, BoneTransform* boneTransformations);
	void LoadAnimation(const char* path, float& ticks, float& ticksPerSecond, int& numBones, BoneTransform** boneTransformations);

	// Bones 
	void ReImportBones(std::string& path, aiBone* bone, JsonParsing& json, std::string& library, std::vector<unsigned int>& bonesUid);
	void ImportBones(std::string& path, aiBone* bone, JsonParsing& json, std::vector<uint>& uids, std::vector<unsigned int>& bonesUid);
	void CreateMetaBones(std::string& library, std::string& assets, uint uid);
	void SaveBone(std::string& name, unsigned int numWeights, float* pos, float* rot, float* scale, std::vector<Weight>& weights);
}


