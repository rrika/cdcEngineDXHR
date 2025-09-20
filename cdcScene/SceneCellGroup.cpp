#include "Scene.h"
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
	m_pSceneB(scene)
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
	m_pCellGroupData = pData[int(type)];
}

SceneCellGroup::SceneCellGroup(Scene *scene, CellGroupData *pData) :
	SceneCellContainer(scene, this),
	m_pSceneB(scene),
	m_pCellGroupData(pData),
	origin{0.f, 0.f, 0.f, 0.f},
	name("CellGroup")
{
	// TODO
}

void SceneCellGroup::Init() {
	m_cells.resize(m_pCellGroupData->header->numTotalCells);
	for (auto& cellPtr : m_cells) {
		cellPtr = new SceneCell(this);
	}
	SceneCellContainer::Init(
		m_cells.data(),
		m_pCellGroupData->header->numToplevelCells,
		m_pCellGroupData->bspNodes,
		m_pCellGroupData->header->numBSPNodes);

	auto *symbols = m_pCellGroupData->symbols;
	if (symbols == s_ppGlobalTable || symbols == s_ppExteriorTable)
		return;

	// TODO

	for (uint32_t i=0; i<m_pCellGroupData->header->numTotalCells; i++)
		m_cells[i]->AddCellData(m_pCellGroupData->cells[i]);

	// TODO
}

IScene *SceneCellGroup::getScene() { return m_pSceneB; }

uint32_t SceneCellGroup::getCellCount() { return m_cells.size(); }

ISceneCell *SceneCellGroup::cellByIndex(uint32_t index) { return m_cells[index]; }

SceneCell *SceneCellGroup::GetCellFromPoint(Vector3 const& point, bool ignoreChildCellContainers) {
	return SceneCellContainer::GetCellFromPoint(point, ignoreChildCellContainers);
}

Vector3 *SceneCellGroup::getOrigin() { return &origin; }

void SceneCellGroup::setUserData(void *) {
	// TODO
}

void SceneCellGroup::setName(const char *newName) { name = newName; }

const char *SceneCellGroup::getName() { return name; }

}
