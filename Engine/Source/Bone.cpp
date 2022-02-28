#include "Bone.h"
#include "Globals.h"

#include "AnimationImporter.h"
#include "Application.h"
#include "FileSystem.h"

#include "glew/include/GL/glew.h"

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