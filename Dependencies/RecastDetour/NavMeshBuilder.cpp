#include "../Globals.h"
#include "NavMeshBuilder.h"
#include "../MO_Pathfinding.h"
#include "RecastNavigation/InputGeom.h"
#include "RecastNavigation/Detour/DetourNavMesh.h"
#include "RecastNavigation/Detour/DetourNavMeshQuery.h"
#include "RecastNavigation/Detour/DetourNavMeshBuilder.h"
#include "RecastNavigation/DebugUtils/SampleInterfaces.h"

#include "RecastNavigation/DebugUtils/DetourDebugDraw.h"
#include "RecastNavigation/DebugUtils/RecastDebugDraw.h"

#include "ImGui/imgui.h"

#include "../Application.h"
#include "../MO_Renderer3D.h"

#include "../RE_Mesh.h"
#include "mmgr/mmgr.h"


inline unsigned int nextPow2(unsigned int v)
{
	v--;
	v |= v >> 1;
	v |= v >> 2;
	v |= v >> 4;
	v |= v >> 8;
	v |= v >> 16;
	v++;
	return v;
}

inline unsigned int ilog2(unsigned int v)
{
	unsigned int r;
	unsigned int shift;
	r = (v > 0xffff) << 4; v >>= r;
	shift = (v > 0xff) << 3; v >>= shift; r |= shift;
	shift = (v > 0xf) << 2; v >>= shift; r |= shift;
	shift = (v > 0x3) << 1; v >>= shift; r |= shift;
	r |= (v >> 1);
	return r;
}

NavMeshBuilder::NavMeshBuilder() :
	m_geom(nullptr),
	m_navMesh(nullptr),
	m_navQuery(nullptr),
	m_keepInterResults(false),
	m_buildAll(true),
	m_totalBuildTimeMs(0),
	m_triareas(nullptr),
	m_solid(nullptr),
	m_chf(nullptr),
	m_cset(nullptr),
	m_pmesh(nullptr),
	m_dmesh(nullptr),
	m_maxTiles(0),
	m_maxPolysPerTile(0),
	m_tileSize(32),
	m_navMeshDrawFlags(DU_DRAWNAVMESH_OFFMESHCONS | DU_DRAWNAVMESH_CLOSEDLIST),
	m_tileBuildTime(0.0f),
	m_tileMemUsage(0.0f),
	m_tileTriCount(0)
{
	ResetCommonSettings();
	m_navQuery = dtAllocNavMeshQuery();
	memset(m_lastBuiltTileBmin, 0, sizeof(m_lastBuiltTileBmin));
	memset(m_lastBuiltTileBmax, 0, sizeof(m_lastBuiltTileBmax));
}

NavMeshBuilder::~NavMeshBuilder()
{
	CleanUp();
	dtFreeNavMesh(m_navMesh);
	m_navMesh = nullptr;
}

void NavMeshBuilder::CleanUp()
{
	delete[] m_triareas;
	m_triareas = nullptr;
	rcFreeHeightField(m_solid);
	m_solid = nullptr;
	rcFreeCompactHeightfield(m_chf);
	m_chf = nullptr;
	rcFreeContourSet(m_cset);
	m_cset = nullptr;
	rcFreePolyMesh(m_pmesh);
	m_pmesh = nullptr;
	rcFreePolyMeshDetail(m_dmesh);
	m_dmesh = nullptr;
}

bool NavMeshBuilder::HandleBuild()
{
	if (!m_geom || !m_geom->getMesh())
	{
		LOG(LogType::L_ERROR, "buildTiledNavigation: No vertices and triangles.");
		return false;
	}

	dtFreeNavMesh(m_navMesh);

	m_navMesh = dtAllocNavMesh();
	if (!m_navMesh)
	{
		LOG(LogType::L_ERROR, "buildTiledNavigation: Could not allocate navmesh.");
		return false;
	}

	dtNavMeshParams params;
	rcVcopy(params.orig, m_geom->getNavMeshBoundsMin());
	params.tileWidth = m_tileSize * m_cellSize;
	params.tileHeight = m_tileSize * m_cellSize;
	params.maxTiles = m_maxTiles;
	params.maxPolys = m_maxPolysPerTile;

	dtStatus status;

	status = m_navMesh->init(&params);
	if (dtStatusFailed(status))
	{
		LOG(LogType::L_ERROR, "buildTiledNavigation: Could not init navmesh.");
		return false;
	}

	status = m_navQuery->init(m_navMesh, 2048);
	if (dtStatusFailed(status))
	{
		LOG(LogType::L_ERROR, "buildTiledNavigation: Could not init Detour navmesh query");
		return false;
	}

	if (m_buildAll)
		BuildAllTiles();

	return true;
}

bool NavMeshBuilder::BuildAllTiles()
{
	if (!m_geom) return false;
	if (!m_navMesh) return false;

	const float* bmin = m_geom->getNavMeshBoundsMin();
	const float* bmax = m_geom->getNavMeshBoundsMax();
	int gw = 0, gh = 0;
	rcCalcGridSize(bmin, bmax, m_cellSize, &gw, &gh);
	const int ts = (int)m_tileSize;
	const int tw = (gw + ts - 1) / ts;
	const int th = (gh + ts - 1) / ts;
	const float tcs = m_tileSize * m_cellSize;

	for (int y = 0; y < th; ++y)
	{
		for (int x = 0; x < tw; ++x)
		{
			m_lastBuiltTileBmin[0] = bmin[0] + x * tcs;
			m_lastBuiltTileBmin[1] = bmin[1];
			m_lastBuiltTileBmin[2] = bmin[2] + y * tcs;

			m_lastBuiltTileBmax[0] = bmin[0] + (x + 1) * tcs;
			m_lastBuiltTileBmax[1] = bmax[1];
			m_lastBuiltTileBmax[2] = bmin[2] + (y + 1) * tcs;

			int dataSize = 0;
			unsigned char* data = BuildTile(x, y, m_lastBuiltTileBmin, m_lastBuiltTileBmax, dataSize);
			if (data)
			{
				// Remove any previous data (navmesh owns and deletes the data).
				m_navMesh->removeTile(m_navMesh->getTileRefAt(x, y, 0), 0, 0);
				// Let the navmesh own the data.
				dtStatus status = m_navMesh->addTile(data, dataSize, DT_TILE_FREE_DATA, 0, 0);
				if (dtStatusFailed(status))
					dtFree(data);
			}
		}
	}

	return true;
}

unsigned char* NavMeshBuilder::BuildTile(const int tx, const int ty, const float* bmin, const float* bmax, int& dataSize)
{
	if (!m_geom || !m_geom->getMesh() || !m_geom->getChunkyMesh())
	{
		LOG(LogType::L_ERROR, "buildNavigation: Input mesh is not specified.");
		return 0;
	}

	m_tileMemUsage = 0.0f;
	m_tileBuildTime = 0.0f;

	rcContext m_ctx;

	CleanUp();

	const float* verts = m_geom->getMesh()->vertices;
	const int nverts = m_geom->getMesh()->vertices_count;
	const int ntris = m_geom->getMesh()->indices_count / 3;
	const rcChunkyTriMesh* chunkyMesh = m_geom->getChunkyMesh();

	// Init build configuration from GUI
	memset(&m_cfg, 0, sizeof(m_cfg));
	m_cfg.cs = m_cellSize;
	m_cfg.ch = m_cellHeight;
	m_cfg.walkableSlopeAngle = m_agentMaxSlope;
	m_cfg.walkableHeight = (int)ceilf(m_agentHeight / m_cfg.ch);
	m_cfg.walkableClimb = (int)floorf(m_agentMaxClimb / m_cfg.ch);
	m_cfg.walkableRadius = (int)ceilf(m_agentRadius / m_cfg.cs);
	m_cfg.maxEdgeLen = (int)(m_edgeMaxLen / m_cellSize);
	m_cfg.maxSimplificationError = m_edgeMaxError;
	m_cfg.minRegionArea = (int)rcSqr(m_regionMinSize);		// Note: area = size*size
	m_cfg.mergeRegionArea = (int)rcSqr(m_regionMergeSize);	// Note: area = size*size
	m_cfg.maxVertsPerPoly = (int)m_vertsPerPoly;
	m_cfg.tileSize = (int)m_tileSize;
	m_cfg.borderSize = m_cfg.walkableRadius + 3; // Reserve enough padding.
	m_cfg.width = m_cfg.tileSize + m_cfg.borderSize * 2;
	m_cfg.height = m_cfg.tileSize + m_cfg.borderSize * 2;
	m_cfg.detailSampleDist = m_detailSampleDist < 0.9f ? 0 : m_cellSize * m_detailSampleDist;
	m_cfg.detailSampleMaxError = m_cellHeight * m_detailSampleMaxError;

	// Expand the heighfield bounding box by border size to find the extents of geometry we need to build this tile.
	//
	// This is done in order to make sure that the navmesh tiles connect correctly at the borders,
	// and the obstacles close to the border work correctly with the dilation process.
	// No polygons (or contours) will be created on the border area.
	//
	// IMPORTANT!
	//
	//   :''''''''':
	//   : +-----+ :
	//   : |     | :
	//   : |     |<--- tile to build
	//   : |     | :  
	//   : +-----+ :<-- geometry needed
	//   :.........:
	//
	// You should use this bounding box to query your input geometry.
	//
	// For example if you build a navmesh for terrain, and want the navmesh tiles to match the terrain tile size
	// you will need to pass in data from neighbour terrain tiles too! In a simple case, just pass in all the 8 neighbours,
	// or use the bounding box below to only pass in a sliver of each of the 8 neighbours.
	rcVcopy(m_cfg.bmin, bmin);
	rcVcopy(m_cfg.bmax, bmax);
	m_cfg.bmin[0] -= m_cfg.borderSize * m_cfg.cs;
	m_cfg.bmin[2] -= m_cfg.borderSize * m_cfg.cs;
	m_cfg.bmax[0] += m_cfg.borderSize * m_cfg.cs;
	m_cfg.bmax[2] += m_cfg.borderSize * m_cfg.cs;

	// Reset build times gathering.

	// Start the build process.

	LOG(LogType::L_NORMAL, "Building navigation:");
	LOG(LogType::L_NORMAL, " - %d x %d cells", m_cfg.width, m_cfg.height);
	LOG(LogType::L_NORMAL, " - %.1fK verts, %.1fK tris", nverts / 1000.0f, ntris / 1000.0f);

	// Allocate voxel heightfield where we rasterize our input data to.
	m_solid = rcAllocHeightfield();
	if (!m_solid)
	{
		LOG(LogType::L_ERROR, "buildNavigation: Out of memory 'solid'.");
		return 0;
	}
	if (!rcCreateHeightfield(&m_ctx, *m_solid, m_cfg.width, m_cfg.height, m_cfg.bmin, m_cfg.bmax, m_cfg.cs, m_cfg.ch))
	{
		LOG(LogType::L_ERROR, "buildNavigation: Could not create solid heightfield.");
		return 0;
	}

	// Allocate array that can hold triangle flags.
	// If you have multiple meshes you need to process, allocate
	// and array which can hold the max number of triangles you need to process.
	m_triareas = new unsigned char[chunkyMesh->maxTrisPerChunk];
	if (!m_triareas)
	{
		LOG(LogType::L_ERROR, "buildNavigation: Out of memory 'm_triareas' (%d).", chunkyMesh->maxTrisPerChunk);
		return 0;
	}

	float tbmin[2], tbmax[2];
	tbmin[0] = m_cfg.bmin[0];
	tbmin[1] = m_cfg.bmin[2];
	tbmax[0] = m_cfg.bmax[0];
	tbmax[1] = m_cfg.bmax[2];
	int cid[512];// TODO: Make grow when returning too many items.
	const int ncid = rcGetChunksOverlappingRect(chunkyMesh, tbmin, tbmax, cid, 512);
	if (!ncid)
		return 0;

	m_tileTriCount = 0;

	for (int i = 0; i < ncid; ++i)
	{
		const rcChunkyTriMeshNode& node = chunkyMesh->nodes[cid[i]];
		const int* ctris = &chunkyMesh->tris[node.i * 3];
		const int nctris = node.n;

		m_tileTriCount += nctris;

		memset(m_triareas, 0, nctris * sizeof(unsigned char));
		rcMarkWalkableTriangles(&m_ctx, m_cfg.walkableSlopeAngle,
			verts, nverts, ctris, nctris, m_triareas);

		if (!rcRasterizeTriangles(&m_ctx, verts, nverts, ctris, m_triareas, nctris, *m_solid, m_cfg.walkableClimb))
			return 0;
	}


	delete[] m_triareas;
	m_triareas = 0;


	// Once all geometry is rasterized, we do initial pass of filtering to
	// remove unwanted overhangs caused by the conservative rasterization
	// as well as filter spans where the character cannot possibly stand.
	rcFilterLowHangingWalkableObstacles(&m_ctx, m_cfg.walkableClimb, *m_solid);
	rcFilterLedgeSpans(&m_ctx, m_cfg.walkableHeight, m_cfg.walkableClimb, *m_solid);
	rcFilterWalkableLowHeightSpans(&m_ctx, m_cfg.walkableHeight, *m_solid);

	// Compact the heightfield so that it is faster to handle from now on.
	// This will result more cache coherent data as well as the neighbours
	// between walkable cells will be calculated.
	m_chf = rcAllocCompactHeightfield();
	if (!m_chf)
	{
		LOG(LogType::L_ERROR, "buildNavigation: Out of memory 'chf'.");
		return 0;
	}
	if (!rcBuildCompactHeightfield(&m_ctx, m_cfg.walkableHeight, m_cfg.walkableClimb, *m_solid, *m_chf))
	{
		LOG(LogType::L_ERROR, "buildNavigation: Could not build compact data.");
		return 0;
	}


	rcFreeHeightField(m_solid);
	m_solid = 0;


	// Erode the walkable area by agent radius.
	if (!rcErodeWalkableArea(&m_ctx, m_cfg.walkableRadius, *m_chf))
	{
		LOG(LogType::L_ERROR, "buildNavigation: Could not erode.");
		return 0;
	}

	// (Optional) Mark areas.
	const ConvexVolume* vols = m_geom->getConvexVolumes();
	for (int i = 0; i < m_geom->getConvexVolumeCount(); ++i)
		rcMarkConvexPolyArea(&m_ctx, vols[i].verts, vols[i].nverts, vols[i].hmin, vols[i].hmax, (unsigned char)vols[i].area, *m_chf);


	// Partition the heightfield so that we can use simple algorithm later to triangulate the walkable areas.
	// There are 3 martitioning methods, each with some pros and cons:
	// 1) Watershed partitioning
	//   - the classic Recast partitioning
	//   - creates the nicest tessellation
	//   - usually slowest
	//   - partitions the heightfield into nice regions without holes or overlaps
	//   - the are some corner cases where this method creates produces holes and overlaps
	//      - holes may appear when a small obstacles is close to large open area (triangulation can handle this)
	//      - overlaps may occur if you have narrow spiral corridors (i.e stairs), this make triangulation to fail
	//   * generally the best choice if you precompute the nacmesh, use this if you have large open areas
	// 2) Monotone partioning
	//   - fastest
	//   - partitions the heightfield into regions without holes and overlaps (guaranteed)
	//   - creates long thin polygons, which sometimes causes paths with detours
	//   * use this if you want fast navmesh generation
	// 3) Layer partitoining
	//   - quite fast
	//   - partitions the heighfield into non-overlapping regions
	//   - relies on the triangulation code to cope with holes (thus slower than monotone partitioning)
	//   - produces better triangles than monotone partitioning
	//   - does not have the corner cases of watershed partitioning
	//   - can be slow and create a bit ugly tessellation (still better than monotone)
	//     if you have large open areas with small obstacles (not a problem if you use tiles)
	//   * good choice to use for tiled navmesh with medium and small sized tiles


	// Prepare for region partitioning, by calculating distance field along the walkable surface.
	//if (!rcBuildDistanceField(&m_ctx, *m_chf))
	//{
	//	LOG(LogType::L_ERROR, "buildNavigation: Could not build distance field.");
	//	return 0;
	//}

	//// Partition the walkable surface into simple regions without holes.
	//if (!rcBuildRegions(&m_ctx, *m_chf, m_cfg.borderSize, m_cfg.minRegionArea, m_cfg.mergeRegionArea))
	//{
	//	LOG(LogType::L_ERROR, "buildNavigation: Could not build watershed regions.");
	//	return 0;
	//}

	if (!rcBuildLayerRegions(&m_ctx, *m_chf, m_cfg.borderSize, m_cfg.minRegionArea))
	{
		LOG(LogType::L_ERROR, "buildNavigation: Could not build layer regions.");
		return 0;
	}

	// Create contours.
	m_cset = rcAllocContourSet();
	if (!m_cset)
	{
		LOG(LogType::L_ERROR, "buildNavigation: Out of memory 'cset'.");
		return 0;
	}
	if (!rcBuildContours(&m_ctx, *m_chf, m_cfg.maxSimplificationError, m_cfg.maxEdgeLen, *m_cset))
	{
		LOG(LogType::L_ERROR, "buildNavigation: Could not create contours.");
		return 0;
	}

	if (m_cset->nconts == 0)
	{
		return 0;
	}

	// Build polygon navmesh from the contours.
	m_pmesh = rcAllocPolyMesh();
	if (!m_pmesh)
	{
		LOG(LogType::L_ERROR, "buildNavigation: Out of memory 'pmesh'.");
		return 0;
	}
	if (!rcBuildPolyMesh(&m_ctx, *m_cset, m_cfg.maxVertsPerPoly, *m_pmesh))
	{
		LOG(LogType::L_ERROR, "buildNavigation: Could not triangulate contours.");
		return 0;
	}

	// Build detail mesh.
	m_dmesh = rcAllocPolyMeshDetail();
	if (!m_dmesh)
	{
		LOG(LogType::L_ERROR, "buildNavigation: Out of memory 'dmesh'.");
		return 0;
	}

	if (!rcBuildPolyMeshDetail(&m_ctx, *m_pmesh, *m_chf,
		m_cfg.detailSampleDist, m_cfg.detailSampleMaxError,
		*m_dmesh))
	{
		LOG(LogType::L_ERROR, "buildNavigation: Could build polymesh detail.");
		return 0;
	}

	rcFreeCompactHeightfield(m_chf);
	m_chf = 0;
	rcFreeContourSet(m_cset);
	m_cset = 0;

	unsigned char* navData = 0;
	int navDataSize = 0;
	if (m_cfg.maxVertsPerPoly <= DT_VERTS_PER_POLYGON)
	{
		if (m_pmesh->nverts >= 0xffff)
		{
			// The vertex indices are ushorts, and cannot point to more than 0xffff vertices.
			LOG(LogType::L_ERROR, "Too many vertices per tile %d (max: %d).", m_pmesh->nverts, 0xffff);
			return 0;
		}

		// Update poly flags from areas.
		for (int i = 0; i < m_pmesh->npolys; ++i)
		{
			if (m_pmesh->areas[i] == RC_WALKABLE_AREA)
				m_pmesh->areas[i] = (int)Walkability::WALKABLE;

			m_pmesh->flags[i] = 1 << m_pmesh->areas[i];
		}

		dtNavMeshCreateParams params;
		memset(&params, 0, sizeof(params));
		params.verts = m_pmesh->verts;
		params.vertCount = m_pmesh->nverts;
		params.polys = m_pmesh->polys;
		params.polyAreas = m_pmesh->areas;
		params.polyFlags = m_pmesh->flags;
		params.polyCount = m_pmesh->npolys;
		params.nvp = m_pmesh->nvp;
		params.detailMeshes = m_dmesh->meshes;
		params.detailVerts = m_dmesh->verts;
		params.detailVertsCount = m_dmesh->nverts;
		params.detailTris = m_dmesh->tris;
		params.detailTriCount = m_dmesh->ntris;
		params.offMeshConVerts = m_geom->getOffMeshConnectionVerts();
		params.offMeshConRad = m_geom->getOffMeshConnectionRads();
		params.offMeshConDir = m_geom->getOffMeshConnectionDirs();
		params.offMeshConAreas = m_geom->getOffMeshConnectionAreas();
		params.offMeshConFlags = m_geom->getOffMeshConnectionFlags();
		params.offMeshConUserID = m_geom->getOffMeshConnectionId();
		params.offMeshConCount = m_geom->getOffMeshConnectionCount();
		params.walkableHeight = m_agentHeight;
		params.walkableRadius = m_agentRadius;
		params.walkableClimb = m_agentMaxClimb;
		params.tileX = tx;
		params.tileY = ty;
		params.tileLayer = 0;
		rcVcopy(params.bmin, m_pmesh->bmin);
		rcVcopy(params.bmax, m_pmesh->bmax);
		params.cs = m_cfg.cs;
		params.ch = m_cfg.ch;
		params.buildBvTree = true;

		if (!dtCreateNavMeshData(&params, &navData, &navDataSize))
		{
			LOG(LogType::L_ERROR, "Could not build Detour navmesh.");
			return 0;
		}
	}
	m_tileMemUsage = navDataSize / 1024.0f;

	// Show performance stats.
	LOG(LogType::L_NORMAL, ">> Polymesh: %d vertices  %d polygons", m_pmesh->nverts, m_pmesh->npolys);

	dataSize = navDataSize;
	return navData;
}

void NavMeshBuilder::ResetCommonSettings()
{
	m_cellSize = 0.3f;
	m_cellHeight = 0.2f;
	m_agentHeight = 2.0f;
	m_agentRadius = 0.6f;
	m_agentMaxClimb = 0.9f;
	m_agentMaxSlope = 45.0f;
	m_regionMinSize = 8;
	m_regionMergeSize = 20;
	m_edgeMaxLen = 12.0f;
	m_edgeMaxError = 1.3f;
	m_vertsPerPoly = 6.0f;
	m_detailSampleDist = 6.0f;
	m_detailSampleMaxError = 1.0f;
}

void NavMeshBuilder::HandleMeshChanged(InputGeom* geom, NavAgent agent)
{
	//Normal sample settings 
	m_geom = geom;

	const BuildSettings* buildSettings = geom->getBuildSettings();
	if (buildSettings)
	{
		//Agent
		m_agentHeight = agent.height;
		m_agentRadius = agent.radius;
		m_agentMaxClimb = agent.stopHeight;
		m_agentMaxSlope = agent.maxSlope;

		//Nav Mesh
		m_cellSize = buildSettings->cellSize;
		m_cellHeight = buildSettings->cellHeight;
		m_regionMinSize = buildSettings->regionMinSize;
		m_regionMergeSize = buildSettings->regionMergeSize;
		m_edgeMaxLen = buildSettings->edgeMaxLen;
		m_edgeMaxError = buildSettings->edgeMaxError;
		m_vertsPerPoly = buildSettings->vertsPerPoly;
		m_detailSampleDist = buildSettings->detailSampleDist;
		m_detailSampleMaxError = buildSettings->detailSampleMaxError;
		m_partitionType = buildSettings->partitionType;
	}

	if (buildSettings && buildSettings->tileSize > 0)
		m_tileSize = buildSettings->tileSize;

	CleanUp();

	dtFreeNavMesh(m_navMesh);
	m_navMesh = nullptr;
}

void NavMeshBuilder::HandleSettings()
{
	if (m_geom)
	{
		char text[64];
		int gw = 0, gh = 0;
		const float* bmin = m_geom->getNavMeshBoundsMin();
		const float* bmax = m_geom->getNavMeshBoundsMax();
		rcCalcGridSize(bmin, bmax, m_cellSize, &gw, &gh);
		const int ts = (int)m_tileSize;
		const int tw = (gw + ts - 1) / ts;
		const int th = (gh + ts - 1) / ts;
		snprintf(text, 64, "Tiles  %d x %d", tw, th);
		//imguiValue(text);

		// Max tiles and max polys affect how the tile IDs are caculated.
		// There are 22 bits available for identifying a tile and a polygon.
		int tileBits = rcMin((int)ilog2(nextPow2(tw * th)), 14);
		if (tileBits > 14) tileBits = 14;
		int polyBits = 22 - tileBits;
		m_maxTiles = 1 << tileBits;
		m_maxPolysPerTile = 1 << polyBits;
		snprintf(text, 64, "Max Tiles  %d", m_maxTiles);
		//imguiValue(text);
		snprintf(text, 64, "Max Polys  %d", m_maxPolysPerTile);
		//imguiValue(text);
	}
	else
	{
		m_maxTiles = 0;
		m_maxPolysPerTile = 0;
	}
}


#ifndef STANDALONE
void NavMeshBuilder::DebugDraw()
{
	if (m_geom != nullptr)
	{
		const float* bmin = m_geom->getNavMeshBoundsMin();
		const float* bmax = m_geom->getNavMeshBoundsMax();
		DrawBoundaries(bmin[0], bmin[1], bmin[2], bmax[0], bmax[1], bmax[2]);
	}

	if (m_navMesh != nullptr)
	{
		DebugDrawGL dd;

		if(m_navMesh != nullptr)
			duDebugDrawNavMeshWithClosedList(&dd, *m_navMesh, *m_navQuery, m_navMeshDrawFlags);
	}
}

void NavMeshBuilder::DrawBoundaries(float minx, float miny, float minz, float maxx, float maxy, float maxz)
{
	//Top
	EngineExternal->moduleRenderer3D->AddDebugLines(float3(minx, miny, minz), float3(maxx, miny, minz), float3(1.0f, 1.0f, 1.0f));
	EngineExternal->moduleRenderer3D->AddDebugLines(float3(maxx, miny, minz), float3(maxx, miny, maxz), float3(1.0f, 1.0f, 1.0f));
	EngineExternal->moduleRenderer3D->AddDebugLines(float3(maxx, miny, maxz), float3(minx, miny, maxz), float3(1.0f, 1.0f, 1.0f));
	EngineExternal->moduleRenderer3D->AddDebugLines(float3(minx, miny, maxz), float3(minx, miny, minz), float3(1.0f, 1.0f, 1.0f));

	//Bottom	
	EngineExternal->moduleRenderer3D->AddDebugLines(float3(minx, maxy, minz), float3(maxx, maxy, minz), float3(1.0f, 1.0f, 1.0f));
	EngineExternal->moduleRenderer3D->AddDebugLines(float3(maxx, maxy, minz), float3(maxx, maxy, maxz), float3(1.0f, 1.0f, 1.0f));
	EngineExternal->moduleRenderer3D->AddDebugLines(float3(maxx, maxy, maxz), float3(minx, maxy, maxz), float3(1.0f, 1.0f, 1.0f));
	EngineExternal->moduleRenderer3D->AddDebugLines(float3(minx, maxy, maxz), float3(minx, maxy, minz), float3(1.0f, 1.0f, 1.0f));

	//Sides		
	EngineExternal->moduleRenderer3D->AddDebugLines(float3(minx, miny, minz), float3(minx, maxy, minz), float3(1.0f, 1.0f, 1.0f));
	EngineExternal->moduleRenderer3D->AddDebugLines(float3(maxx, miny, minz), float3(maxx, maxy, minz), float3(1.0f, 1.0f, 1.0f));
	EngineExternal->moduleRenderer3D->AddDebugLines(float3(maxx, miny, maxz), float3(maxx, maxy, maxz), float3(1.0f, 1.0f, 1.0f));
	EngineExternal->moduleRenderer3D->AddDebugLines(float3(minx, miny, maxz), float3(minx, maxy, maxz), float3(1.0f, 1.0f, 1.0f));
}


void NavMeshBuilder::OnEditor()
{
	if (ImGui::CollapsingHeader("NavMeshBuilder"))
	{
		ImGui::Text("NaveMesh Geometry");
		if (m_geom == nullptr)
		{
			ImGui::SameLine();
			ImGui::Text("No Geometry added");
			return;
		}

		//ImGui::SameLine();
		ImGui::Text("Verts: %d", m_geom->getMesh()->vertices_count);
		ImGui::Text("Indices: %d", m_geom->getMesh()->indices_count);

		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();

		ImGui::Text("Max Tiles: %i", m_maxTiles);
		ImGui::Text("Max Polys per tiles: %i", m_maxPolysPerTile);
		ImGui::Text("Cell size: %.4f", m_cellSize);
		ImGui::Text("Cell Height: %.4f", m_cellHeight);

		ImGui::Separator();

		if (m_pmesh != nullptr)
		{
			ImGui::Separator();
			ImGui::Text("PolyMesh");

			ImGui::Text("Verts: %i", m_pmesh->nverts);
			ImGui::Text("Polys: %i", m_pmesh->npolys);
		}
		//ImGui::Text(": %i", m_cellSize);
	}
}
#endif // !STANDALONE

dtNavMesh* NavMeshBuilder::GetNavMesh() { return m_navMesh; }

void NavMeshBuilder::SetNavMesh(dtNavMesh* newNavMesh)
{
	if (m_navMesh != nullptr)
		dtFreeNavMesh(m_navMesh);

	m_navMesh = newNavMesh;
	m_navQuery->init(m_navMesh, 2048);
}

dtNavMeshQuery* NavMeshBuilder::GetNavMeshQuery() { return m_navQuery; }

void NavMeshBuilder::CollectSettings(BuildSettings& settings)
{
	settings.cellSize = m_cellSize;
	settings.cellHeight = m_cellHeight;
	settings.agentHeight = m_agentHeight;
	settings.agentRadius = m_agentRadius;
	settings.agentMaxClimb = m_agentMaxClimb;
	settings.agentMaxSlope = m_agentMaxSlope;
	settings.regionMinSize = m_regionMinSize;
	settings.regionMergeSize = m_regionMergeSize;
	settings.edgeMaxLen = m_edgeMaxLen;
	settings.edgeMaxError = m_edgeMaxError;
	settings.vertsPerPoly = m_vertsPerPoly;
	settings.detailSampleDist = m_detailSampleDist;
	settings.detailSampleMaxError = m_detailSampleMaxError;
	settings.partitionType = m_partitionType;
}

void NavMeshBuilder::SetSettings(BuildSettings& settings)
{
	m_cellSize = settings.cellSize;
	m_cellHeight = settings.cellHeight;
	m_agentHeight = settings.agentHeight;
	m_agentRadius = settings.agentRadius;
	m_agentMaxClimb = settings.agentMaxClimb;
	m_agentMaxSlope = settings.agentMaxSlope;
	m_regionMinSize = settings.regionMinSize;
	m_regionMergeSize = settings.regionMergeSize;
	m_edgeMaxLen = settings.edgeMaxLen;
	m_edgeMaxError = settings.edgeMaxError;
	m_vertsPerPoly = settings.vertsPerPoly;
	m_detailSampleDist = settings.detailSampleDist;
	m_detailSampleMaxError = settings.detailSampleMaxError;
	m_partitionType = settings.partitionType;
}

void NavMeshBuilder::SetGeometry(InputGeom* geom)
{
	m_geom = geom;
}

