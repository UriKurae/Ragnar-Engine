#pragma once

#include "assimp/cfileio.h"
#include "PhysFS/include/physfs.h"
#include "Globals.h"

size_t AssimpWrite(aiFile* file, const char* data, size_t size, size_t chunks);

size_t AssimpRead(aiFile* file, char* data, size_t size, size_t chunks);

size_t AssimpTell(aiFile* file);

size_t AssimpSize(aiFile* file);

void AssimpFlush(aiFile* file);

aiReturn AssimpSeek(aiFile* file, size_t pos, aiOrigin from);

aiFile* AssimpOpen(aiFileIO* io, const char* name, const char* format);

void AssimpClose(aiFileIO* io, aiFile* file);