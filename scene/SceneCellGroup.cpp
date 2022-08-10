#include "SceneCell.h"
#include "SceneCellGroup.h"
#include "cdcSceneCookdata.h"

namespace cdc {

static CellGroupDataHeader s_globalGroupHeader = {
	.numTotalCells = 1,
	.numToplevelCells = 1,
	.numBSPNodes = 0,
	.numSymbols = 1
};

static CellGroupDataHeader s_exteriorGroupHeader = {
	.numTotalCells = 1,
	.numToplevelCells = 1,
	.numBSPNodes = 0,
	.numSymbols = 1
};

static const char *s_ppGlobalTable[1] = {"__global"};
static const char *s_ppExteriorTable[1] = {"__exterior"};

static CellGroupData s_globalGroupData = {
	.header = &s_globalGroupHeader,
	.symbols = s_ppGlobalTable,
};

static CellGroupData s_exteriorGroupData = {
	.header = &s_exteriorGroupHeader,
	.symbols = s_ppExteriorTable,
};

SceneCellGroup::SceneCellGroup(Scene *scene, SpecialType type) :
	SceneCellContainer(scene, this),
	scene1C(scene)
{
	static const char *pNames[2] = {
		"Global Cellgroup",
		"Exterior Cellgroup"
	};
	static CellGroupData *pData[2] = {
		&s_globalGroupData,
		&s_exteriorGroupData
	};
	name = pNames[int(type)];
	cellGroupData = pData[int(type)];
}

// SceneCellGroup::SceneCellGroup(Scene*, CellGroupData*) {
// 	// TODO
// }

void SceneCellGroup::allocateCells() {
	cells.resize(cellGroupData->header->numTotalCells);
	for (auto& cellPtr : cells) {
		cellPtr = new SceneCell(this);
	}
}

}
