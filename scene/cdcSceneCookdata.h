#pragma once

namespace cdc {

class ISceneCell;
class ResolveObject;

struct CellStreamGroupData {
	const char *name;
	uint32_t numCells;
	ISceneCell *cells;
	const char *streamFileName;
	ResolveObject *resolveObject;
};

}