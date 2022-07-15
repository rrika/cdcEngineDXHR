#pragma once

namespace cdc {

struct StencilSettings {
	uint16_t front;
	uint8_t stencilRef;
	uint8_t stencilReadMask;
	uint16_t back;
	uint8_t byte6;
	uint8_t byte7;
	uint8_t stencilWriteMask;
	uint8_t byte9;
	uint8_t byteA;
	uint8_t byteB;
	uint32_t dwordC;
};

}
