#pragma once
#include <cstdint>
#include "../math/Math.h"

namespace cdc { class Mesh; }

struct StreamUnitPortal;

struct Terrain {
	uint16_t word0;
	uint16_t num_signalConnections;
	StreamUnitPortal *signalConnections;
	uint32_t countWaterSurfaces;
	void **pptrC;
	void *ptr10;
	uint32_t numMatricesAndCDLinks;
	cdc::Matrix *matricesAndCDLinks;
	uint32_t numCollisionData;
	cdc::Mesh *collisionData;
};
