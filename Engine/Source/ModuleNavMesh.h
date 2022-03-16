#pragma once
#include "Module.h"
#include "Recast/Recast.h"
#include "Detour/DetourNavMesh.h"
#include "Detour/DetourNavMeshQuery.h"

#include "MathGeoLib.h"

class GameObject;
class InputGeom;
class NavMeshBuilder;

enum class PathType
{
	SMOOTH,
	STRAIGHT
};

struct NavAgent
{
	NavAgent();

	float radius = 0.0f;
	float height = 0.0f;
	float maxClimb = 0.0f;
	int maxSlope = 0;

	float speed = 0.0f;
	float angularSpeed = 0.0f;
	float acceleration = 0.0f;
	float stoppingDistance = 0;

	PathType pathType = PathType::STRAIGHT;
};

struct Pathfinder
{
	Pathfinder();
	~Pathfinder();
	void Init(NavMeshBuilder* builder);

	bool CalculatePath();
	bool CalculatePath(float3 origin, float3 destination, std::vector<float3>& path);
	void RenderPath();

	PathType pathType;

	dtNavMesh* m_navMesh = nullptr;
	dtNavMeshQuery* m_navQuery = nullptr;
	dtQueryFilter m_filter;
	NavMeshBuilder* m_navMeshBuilder = nullptr;

	int m_straightPathOptions;

	static const int MAX_POLYS = 256;
	static const int MAX_SMOOTH = 2048;

	dtPolyRef m_startRef;
	dtPolyRef m_endRef;
	dtPolyRef m_polys[MAX_POLYS];
	dtPolyRef m_parent[MAX_POLYS];
	int m_npolys;

	int m_nstraightPath;
	float m_straightPath[MAX_POLYS * 3];
	unsigned char m_straightPathFlags[MAX_POLYS];
	dtPolyRef m_straightPathPolys[MAX_POLYS];

	int m_nsmoothPath;
	float m_smoothPath[MAX_SMOOTH * 3];

	float3 startPosition;
	bool startPosSet;
	float3 endPosition;
	bool endPosSet;

	int m_pathIterNum;
	float m_polyPickExt[3];
};

struct BuildSettings
{
	// Cell size in world units
	float cellSize;
	// Cell height in world units
	float cellHeight;
	// Agent height in world units
	float agentHeight;
	// Agent radius in world units
	float agentRadius;
	// Agent max climb in world units
	float agentMaxClimb;
	// Agent max slope in degrees
	float agentMaxSlope;
	// Region minimum size in voxels.
	// regionMinSize = sqrt(regionMinArea)
	float regionMinSize;
	// Region merge size in voxels.
	// regionMergeSize = sqrt(regionMergeArea)
	float regionMergeSize;
	// Edge max length in world units
	float edgeMaxLen;
	// Edge max error in voxels
	float edgeMaxError;
	float vertsPerPoly;
	// Detail sample distance in voxels
	float detailSampleDist;
	// Detail sample max error in voxel heights.
	float detailSampleMaxError;
	// Partition type, see SamplePartitionType
	int partitionType;
	// Size of the tiles in voxels
	float tileSize;
};

class ModuleNavMesh : public Module {
public:
	ModuleNavMesh(bool start_enabled = true);
	~ModuleNavMesh();

	bool Start() override;
	bool Update(float dt) override;

	bool LoadConfig(JsonParsing& node) override;
	bool SaveConfig(JsonParsing& node) override;

	void ClearNavMeshes();
	bool IsWalkable(float x, float z, float3& hitPoint);

	bool CleanUp() override;

	void BakeNavMesh();
	void AddGameObjectToNavMesh(GameObject* objectToAdd);
	inline NavMeshBuilder* GetNavMeshBuilder() { return navMeshBuilder; };
	const inline InputGeom* GetInputGeom() const { return geometry; };
	BuildSettings* GetBuildSettings() { return buildSettings; };

public:
	Pathfinder pathfinder;

private:
	NavMeshBuilder* navMeshBuilder = nullptr;
	InputGeom* geometry = nullptr;
	BuildSettings* buildSettings = nullptr;
};
