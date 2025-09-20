#pragma once
#include <cstdint>

namespace cdc {

class IRenderTerrain;
class ISceneCell;
class RenderMesh;
class ResolveObject;
struct CellBlobPortals;
struct CellData;
struct CellStreamData;
struct CellStreamGroupData;
struct SceneCellBSPNode;

struct CellGroupDataHeader { // 50
	uint32_t numTotalCells; // 0
	uint32_t numToplevelCells; // 4
	uint32_t numBSPNodes; // 8
	uint32_t numStreamGroups; // C
	uint32_t numSymbols; // 10
	uint32_t numPortalSymbols; // 14
	uint32_t symbolTableSize; // 18
};

struct CellGroupData { // 61
	CellGroupDataHeader *header; // 0
	uint32_t admd_maybe; // 4
	SceneCellBSPNode *bspNodes; // 8
	CellStreamGroupData *streamgroups; // C
	const char **symbols; // 10
	CellData **cells; // 14
	CellStreamData *void_terrain_maybe; // 18
	CellStreamData *exterior_terrain_maybe; // 1C
};

struct CellStreamGroupData { // 79
	const char *name;
	uint32_t numCells;
	ISceneCell *cells;
	const char *streamFileName;
	ResolveObject *resolveObject;
};

struct CellDataHeader { // line 121
	char *name;
	uint32_t dword4;
	uint32_t dword8;
	uint32_t dwordC;
	float vec10[4]; // bbox.center
	float vec20[4]; // bbox.extent
	uint32_t dword30;
	uint16_t word34;
	uint16_t word36;
	uint16_t sub4Count;
	uint16_t word3A;
	uint32_t dword3C;
	uint16_t word40;
	uint16_t numBSPNodes;
	uint16_t numChildCells; // 44
	uint16_t word46;
	uint16_t firstChildIndex; // 48
	uint16_t word4A;
	uint16_t word4C;
	uint16_t word4E;
	CellStreamGroupData *streamGroup50;
	uint32_t streamGroupDtp54;
	void *dword58;
	uint32_t pmatrix_or_imfref_5C;
};

static_assert(sizeof(CellDataHeader) == 0x60);

struct CellTerrainData {
	IRenderTerrain *pTerrain;
	uint32_t flags;
	uint32_t lightMask;
	void *pUserData;
};

struct CellData { // line 157
	CellDataHeader *pHeader;
	CellTerrainData *pTerrainData;
	uint32_t dword8;
	uint32_t dwordC;
	uint32_t dword10;
	uint32_t dword14;
	uint32_t dword18;
	SceneCellBSPNode *bspNodes;
	RenderMesh *renderMesh; // 20
	CellBlobPortals *portals;
};


}
