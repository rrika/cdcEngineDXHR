#pragma once
#include <cstdint>

namespace cdc { class RenderMesh; }

namespace dtp {

struct IntermediateMesh {
	uint32_t m_type;
	cdc::RenderMesh *pRenderModel;
	// unknown
};

}
