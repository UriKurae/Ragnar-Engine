#pragma once

#include "JsonParsing.h"

#include "PhysFS/include/physfs.h"
#include "assimp/cfileio.h"

typedef unsigned int uint;

class FileSystem
{
public:
	FileSystem() {}
	FileSystem(const char* assetsPath);

	~FileSystem();

	bool Init(JsonParsing& node);

	bool CleanUp();

	bool AddPath(const char* path);

	uint Load(const char* file, char** buffer);
	uint Save(const char* file, const void* buffer, unsigned int size, bool append = false);

	inline aiFileIO* GetAssimpIO() const { return assimpIO; }

	inline const char* GetBasePath() const { return PHYSFS_getBaseDir(); }
	inline const char* GetWritePath() const { return PHYSFS_getWriteDir(); }
	const char* GetReadPaths() const;

private:
	void CreateAssimpIO();

private:
	const char* name;

	aiFileIO* assimpIO;
};