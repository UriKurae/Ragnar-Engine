#pragma once
#include "Module.h"
#include "RecastDetour/Recast/Recast.h"
#include "RecastDetour/Detour/DetourNavMesh.h"
#include "RecastDetour/Detour/DetourNavMeshQuery.h"

#include "MathGeoLib.h"
#include <string>

class dtNavMesh;
class dtNavMeshQuery;
class dtQueryFilter;
class GameObject;
class InputGeom;
class NavMeshBuilder;

class ModuleNavMesh : public Module
{
public:
	
	ModuleNavMesh(bool startEnabled = true);
	~ModuleNavMesh();

	bool Init(JsonParsing& node) override;
	bool PreUpdate(float dt) override;
	bool CleanUp();

	bool LoadConfig(JsonParsing& node) override;
	bool SaveConfig(JsonParsing& node) override;
};