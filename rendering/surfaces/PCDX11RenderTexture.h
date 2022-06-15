#pragma once
#include "PCDX11BaseTexture.h"
#include "TextureMap.h"

namespace cdc {

class IRenderSurface;

class PCDX11RenderTexture :
	public TextureMap,
	public PCDX11BaseTexture
{
public:
	PCDX11RenderDevice *renderDevice; // 2C
	// uint32_t dword30;
	// uint32_t dword34;
	ID3D11Resource *resource = nullptr; // 38
	ID3D11View *view = nullptr; // 3C (color or depth)
	ID3D11ShaderResourceView *shaderResourceView = nullptr; // 40
	ID3D11DepthStencilView *depthStencilView = nullptr; // 44
	ID3D11UnorderedAccessView *unorderedAccessView = nullptr; // 48
	// uint8_t byte4C;
	// uint8_t byte4D;
	// uint8_t byte4E;
	// uint8_t byte4F;
	uint16_t shortWidth;
	uint16_t shortHeight;
	uint32_t isDepthBuffer = 0;
	// PCDX11RenderBuffer *renderBuffer; // 58

	PCDX11RenderTexture(
		uint16_t width, uint16_t height,
		uint32_t unknown1, uint32_t isDepthBuffer,
		PCDX11RenderDevice *renderDevice, uint32_t unknown2);

	void ensureRenderTargetView();
	void ensureBuffer();
	ID3D11View *getView();
	void initForRenderTarget(IRenderSurface *renderSurface, uint32_t format, ID3D11Texture2D *texture);

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

	ID3D11RenderTargetView *createRenderTargetView();
	ID3D11DepthStencilView *createDepthStencilView();
};

}
