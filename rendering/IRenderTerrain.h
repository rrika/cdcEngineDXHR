#pragma once
#include "RenderResource.h"

namespace cdc {

class IRenderTerrain :
	public RenderResource
{
public:

	struct Node { // sizeof(Node) == 32
		uint32_t dword0;
		uint32_t dword4;
		uint32_t dword8;
		uint32_t dwordC;
		uint32_t dword10;
		uint32_t dword14;
		uint32_t dword18;
		uint16_t m_subTreeSize;
		uint8_t m_numChildren;
		uint8_t m_isGeometry;
	};

	// virtual void method_18() = 0;
	// virtual void method_1C() = 0;
	// virtual void Render(/*TODO*/) = 0;
	// virtual void method_24() = 0;
};

}
