#pragma once
#include <cstdint>
#include "cdcMath/Math.h"

namespace cdc { struct Mesh; struct MeshInstance; }

struct StreamUnitPortal;

struct Terrain {
	uint16_t word0;
	uint16_t num_signalConnections;
	StreamUnitPortal *signalConnections;
	uint32_t countWaterSurfaces;
	void **pptrC;
	void *ptr10;
	uint32_t numMeshInstances;
	cdc::MeshInstance *meshInstances;
	uint32_t numMeshes;
	cdc::Mesh *meshes; // 20
};
