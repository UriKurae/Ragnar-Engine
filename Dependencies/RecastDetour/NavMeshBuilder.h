#pragma once

#include "RecastNavigation/Recast/Recast.h"
#include "RecastNavigation/Detour/DetourNavMesh.h"
#include "RecastNavigation/Detour/DetourNavMeshQuery.h"
#include "RecastNavigation/InputGeom.h"
#include "../MO_Pathfinding.h"

enum class Walkability
{
	WALKABLE,
	NOT_WALKABLE
};

class NavMeshBuilder {
public:
	NavMeshBuilder();
	~NavMeshBuilder();

	void CleanUp();

	bool HandleBuild();
	bool BuildAllTiles();
	unsigned char* BuildTile(const int tx, const int ty, const float* bmin, const float* bmax, int& dataSize);
	void ResetCommonSettings();
	void HandleMeshChanged(InputGeom* geom, NavAgent agent);
	void HandleSettings();

	void DebugDraw();
	void DrawBoundaries(float minx, float miny, float minz, float maxx, float maxy, float maxz);
	void OnEditor();

	dtNavMesh* GetNavMesh();
	void SetNavMesh(dtNavMesh* newNavMesh);

	dtNavMeshQuery* GetNavMeshQuery();

	void CollectSettings(BuildSettings& settings);
	void SetSettings(BuildSettings& settings);

	void SetGeometry(InputGeom* geom);

private:
	class InputGeom* m_geom;
	class dtNavMesh* m_navMesh;
	class dtNavMeshQuery* m_navQuery;

	unsigned char m_navMeshDrawFlags;

	bool m_keepInterResults;
	bool m_buildAll;
	float m_totalBuildTimeMs;

	unsigned char* m_triareas;
	rcHeightfield* m_solid;
	rcCompactHeightfield* m_chf;
	rcContourSet* m_cset;
	rcPolyMesh* m_pmesh;
	rcConfig m_cfg;
	rcPolyMeshDetail* m_dmesh;

	float m_cellSize			 = 0.0f;
	float m_cellHeight			 = 0.0f;
	float m_agentHeight			 = 0.0f;
	float m_agentRadius			 = 0.0f;
	float m_agentMaxClimb		 = 0.0f;
	float m_agentMaxSlope		 = 0.0f;
	float m_regionMinSize		 = 0.0f;
	float m_regionMergeSize      = 0.0f;
	float m_edgeMaxLen		     = 0.0f;
	float m_edgeMaxError		 = 0.0f;
	float m_vertsPerPoly		 = 0.0f;
	float m_detailSampleDist	 = 0.0f;
	float m_detailSampleMaxError = 0.0f;
	int m_partitionType			 = 0.0f;

	//Tile Settings
	int   m_maxTiles;
	int   m_maxPolysPerTile;
	float m_tileSize;

	unsigned int m_tileCol;
	float m_lastBuiltTileBmin[3];
	float m_lastBuiltTileBmax[3];
	float m_tileBuildTime;
	float m_tileMemUsage;
	int m_tileTriCount;

public:
	BuildSettings buildSettings;
};
