#pragma once
#include <cstdint>

namespace cdc {

// see PCDX11StateManager::setStencil for decoding

struct StencilParams {
	uint32_t m_frontParams; // 0
		// uint16_t m_frontEnabled  : 1;
		// uint16_t m_frontFunc     : 3;
		// uint16_t m_frontPassOp   : 4;
		// uint16_t m_frontFailOp   : 4;
		// uint16_t m_frontZFailOp  : 4;
		// uint8_t m_frontRef;
		// uint8_t m_frontReadMask;
	uint32_t m_backParams; // 4
		// uint16_t m_backEnabled  : 1;
		// uint16_t m_backFunc     : 3;
		// uint16_t m_backPassOp   : 4;
		// uint16_t m_backFailOp   : 4;
		// uint16_t m_backZFailOp  : 4;
		// uint8_t unused6;
		// uint8_t unused7;
	uint32_t m_writeMasks; // 8
		// uint8_t m_frontWriteMask;
		// uint8_t unused9;
		// uint8_t unusedA;
		// uint8_t unusedB;
	uint32_t m_hiStencil; // C
};

}
