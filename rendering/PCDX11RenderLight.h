#pragma once
#include "CommonRenderLight.h"

namespace cdc {

struct RenderLightData;

class PCDX11RenderLight : public CommonRenderLight {
public:
	uint32_t dwordEC;
	uint8_t fF0[32];
	RenderLightData *renderLightData110;
	uint8_t byte114;
	uint8_t f115[3];
	uint8_t byte118;
	uint8_t f119[55];
	float vec150[4];
};

}
