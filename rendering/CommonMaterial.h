#pragma once
#include "IMaterial.h"
#include <cstdint>

namespace cdc {

struct StencilSettings {
	uint32_t data[4];
};

struct MaterialBlobSub {
	// TODO
};

struct MaterialBlob {
	uint16_t word0;
	uint16_t word2;
	int dword4;
	int renderTargetWriteMask;
	int blendState;
	int dword10;
	uint16_t word14;
	uint16_t word16;
	int dword18; // flags, bit 11 controls renderTwice in ModelDrawable
	int dword1C;
	int dword20;
	int dword24;
	uint8_t byte_28;
	uint8_t byte_29;
	uint8_t byte_2A;
	uint8_t byte_2B;
	int negDepthBias;
	float negSlopeScaledDepthBias;
	StencilSettings stencilSettings34;
	int name_44;
	int dword48;
	MaterialBlobSub *subMat4C[16];
};

class CommonMaterial : public IMaterial {
	uint32_t dword4;
	uint32_t dword8;
public:
	MaterialBlob *materialBlob = nullptr;
public:
	void method_04() override;
	void method_0C() override;
	void method_10() override;
	~CommonMaterial() = default;
	virtual void method_18() = 0;
};

}
