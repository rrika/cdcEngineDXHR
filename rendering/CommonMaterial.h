#pragma once
#include <cstdint>
#include "IMaterial.h"
#include "VertexAttribute.h"

namespace cdc {

class IShaderLib;
class TextureMap;

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

struct MaterialTexRef {
	TextureMap *tex;
	float unknown4;
	uint32_t dword8;
	uint8_t fallbackIndex;
	uint8_t slotIndex;
	uint16_t filter;
};

struct MaterialBlobSub {
	IShaderLib *shaderPixel;
	IShaderLib *shaderVertex;
	IShaderLib *shaderHull;
	IShaderLib *shaderDomain;
	uint32_t dword10;

	uint8_t psByte14;
	uint8_t psRefIndexEndB;
	uint8_t psRefIndexEndA;
	uint8_t psRefIndexBeginB;
	MaterialTexRef *psTextureRef;
	uint32_t psBufferSize;
	char *psBufferData;

	uint8_t vsByte24;
	uint8_t vsRefIndexEndB; // 25
	uint8_t vsRefIndexEndA; // 26
	uint8_t vsRefIndexBeginB; // 27
	MaterialTexRef *vsTextureRef; // 28
	uint32_t vsBufferSize; // 2C
	char *vsBufferData; // 30

	uint8_t psBufferFirstRow; // 34
	uint8_t psBufferNumRows; // 35

	uint8_t vsBufferFirstRow; // 36
	uint8_t vsBufferNumRows; // 37

	VertexAttributeLayoutB *layout38;
	VertexAttributeLayoutB *layout3C;
	VertexAttributeLayoutB *layout40;
	uint32_t dword44;
	uint32_t dword48;
	uint32_t dword4C;
	uint32_t dword50;
	uint32_t dword54;
};

struct MaterialBlob {
	uint16_t word0;
	uint16_t word2;
	int dword4;
	int renderTargetWriteMask;
	int blendStateC; // for submat 0, 1, 5
	int dword10;
	uint16_t word14;
	uint16_t word16;
	int dword18; // flags, bit 11 controls renderTwice in ModelDrawable
	int dword1C;
	int dword20;
	uint32_t blendState24; // for submat 4
	uint8_t byte_28;
	uint8_t alphaThreshold; // 29
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
