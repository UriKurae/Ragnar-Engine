#include "Animation.h"
#include "Globals.h"

#include "AnimationImporter.h"
#include "Application.h"
#include "FileSystem.h"
#include "Bone.h"

#include "glew/include/GL/glew.h"

#include "Profiling.h"

Animation::Animation(uint uid, std::string& assets, std::string& library) : Resource(uid, ResourceType::ANIMATION, assets, library)
{
	std::string metaPath = ANIMATIONS_FOLDER + std::string("anim_") + std::to_string(uid) + ".meta";
	AnimationImporter::CreateMetaAnimation(metaPath, assetsPath, uid);
	name = assets;
	app->fs->GetFilenameWithoutExtension(name);
	name = name.substr(name.find_first_of("__") + 2, name.length());
}

Animation::~Animation()
{
	RELEASE_ARRAY(boneTranformations);
}

void Animation::Load()
{
	if (id == 0)
	{
		AnimationImporter::LoadAnimation2(libraryPath.c_str(), ticks, ticksPerSecond, bones);
	}
}

void Animation::UnLoad()
{
	if (id != 0)
	{
		id = 0;
		ticks = 0;
		ticksPerSecond = 0;
		RELEASE_ARRAY(boneTranformations);
	}
}

BoneTransform::~BoneTransform()
{
	RELEASE_ARRAY(posKeysValues);
	RELEASE_ARRAY(posKeysTimes);

	RELEASE_ARRAY(scaleKeysValues);
	RELEASE_ARRAY(scaleKeysTimes);

	RELEASE_ARRAY(rotKeysValues);
	RELEASE_ARRAY(rotKeysTimes);
}
