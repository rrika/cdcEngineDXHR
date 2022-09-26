#pragma once
#include <cstdint>
#include "PCDX11InternalResource.h"

struct ID3D11Buffer;

namespace cdc {

// dummy
class PCDX11StaticPool : public PCDX11InternalResource {
public:
	struct Allocation { // sizeof(Allocation) == 16
		uint32_t offset; // 0
		uint32_t totalSize; // 4
		ID3D11Buffer *buffer; // 8
	};

	bool internalCreate() override { return true; };
	void internalRelease() override { return; };
};

}
