#pragma once
#include "PCDX11BaseTexture.h"
#include "TextureMap.h"

namespace cdc {

class PCDX11RenderTexture :
	public TextureMap,
	public PCDX11BaseTexture
{
public:
	uint32_t width; // 18
	uint32_t height; // 1C
	PCDX11RenderDevice *renderDevice; // 2C
	// uint32_t dword30;
	// uint32_t dword34;
	ID3D11Resource *resource; // 38
	ID3D11View *view; // 3C (color or depth)
	ID3D11ShaderResourceView *shaderResourceView; // 40
	ID3D11UnorderedAccessView *unorderedAccessView; // 44
	// uint32_t dword48;
	// uint8_t byte4C;
	// uint8_t byte4D;
	// uint8_t byte4E;
	// uint8_t byte4F;
	// uint16_t word50;
	// uint16_t word52;
	// uint32_t dword54;
	// PCDX11RenderBuffer *renderBuffer; // 58

	void ensureRenderTargetView();
	void ensureBuffer();
	ID3D11View *getView();

	void resRegisterAtRenderDevice() override;
	void resFill(void* src, size_t size, size_t offset) override;
	char *resGetBuffer() override;
	void resConstruct() override;
	// void resMethod10();
	// ~RenderResource() override;

	uint32_t getWidth() override;
	uint32_t getHeight() override;
	// void method_20() override;
	// void method_24() override;
	// void method_28() override;
	// void method_2C() override;

	ID3D11Resource *getTextureResource() override;
	ID3D11ShaderResourceView *createShaderResourceView() override;
	ID3D11UnorderedAccessView *createUnorderedAccessView() override;
};

}
