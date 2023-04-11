#pragma once
#include "RenderResource.h"

namespace cdc {

class IRenderTerrain :
	public RenderResource
{
public:

	struct Node { // sizeof(Node) == 32
		float center[3];
		uint32_t dwordC;
		float extents[3];
		uint16_t m_subTreeSize;
		uint8_t m_numChildren;
		uint8_t m_isGeometry;
	};

	virtual Node *GetNodes() = 0;
	virtual uint32_t NumNodes() = 0;
	// virtual void method_1C() = 0;
	// virtual void Render(/*TODO*/) = 0;
	// virtual void method_24() = 0;
};

}
