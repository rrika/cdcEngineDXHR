#pragma once
#include <cstdint>
#include "cdc/dtp/admd.h"
#include "cdcScene/cdcScene.h"
#include "Terrain.h"

namespace dtp { struct UnitData; }

namespace cdc {

struct CellGroupData;
class ResolveObject;

struct Level { // UnitBlob
	Terrain *terrain; // 0
	float float4; // waterZLevel
	uint32_t dword8_deref2C; // worldName
	uint32_t dwordC; // SignalIDList
	uint32_t version; // 10
	uint32_t dword14;
	uint32_t dword18;
	uint32_t dword1C;
	uint32_t dword20; // unused?
	uint32_t dword24; // unused?
	uint32_t dword28; // unused?
	dtp::UnitData *unitData; // 2C
	dtp::ADMD *admdData; // 30
	uint32_t dword34; // unused?
	uint32_t dword38; // unused?
	uint32_t dword3C; // unused?
	uint32_t dword40;
	uint32_t dword44;
	uint32_t dword48;
	uint32_t dword4C;
	CellGroupData *sub50;
};

}

struct StreamUnit {
	uint32_t StreamUnitID; // 0
	uint8_t used; // 8
	cdc::Level *level; // C
	uint8_t name[128]; // 10
	cdc::ResolveObject *resolveObject_streamGroup; // 90
	cdc::ResolveObject *resolveObject; // 98
	cdc::ISceneCellGroup *sceneCellGroup; // D4
	StreamUnit *coreUnit; // D8

	int introShowRange[2] = {0, 99999};
	int imfShowRange[2] = {0, 0};
};
