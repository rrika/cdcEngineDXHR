#pragma once
#include <cstdint>

namespace dtp { struct IntermediateMesh; }

namespace cdc {

enum IMFType { // 14
	IMFType_Embedded = 0,
	IMFType_TerrainInstance = 1,
	IMFType_ModelInstance = 2
};

dtp::IntermediateMesh *GetIMFPointerFromId(uint32_t);

}
