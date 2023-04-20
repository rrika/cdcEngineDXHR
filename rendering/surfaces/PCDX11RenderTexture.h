#pragma once
#include "../Types.h"
#include "PCDX11BaseTexture.h"
#include "TextureMap.h"

struct ID3D11Resource;
struct ID3D11ShaderResourceView;

namespace cdc {

class IRenderSurface;

class PCDX11RenderTexture :
	public TextureMap,
	public PCDX11BaseTexture
{
public:
	PCDX11RenderDevice *renderDevice; // 2C
	uint32_t flags; // 30
	IRenderSurface *originRenderSurface = nullptr; // 34
	ID3D11Resource *resource = nullptr; // 38
	ID3D11View *view = nullptr; // 3C (color or depth)
	ID3D11ShaderResourceView *shaderResourceView = nullptr; // 40
	ID3D11DepthStencilView *depthStencilView = nullptr; // 44 (read-only version of view)
	ID3D11UnorderedAccessView *unorderedAccessView = nullptr; // 48
	bool registeredForDeletionAfterFrame = false; // 4C
	bool borrowedResource = false; // 4D
	uint8_t sampleCount = 0; // 4E, not setting this to 1 or higher causes issues
	uint8_t sampleQuality = 0; // 4F
	uint16_t shortWidth; // 50
	uint16_t shortHeight; // 52
	uint32_t isDepthBuffer = 0; // 54
	// PCDX11RenderBuffer *renderBuffer; // 58

	PCDX11RenderTexture(
		uint16_t width, uint16_t height,
		uint32_t unknown1, uint32_t isDepthBuffer,
		PCDX11RenderDevice *renderDevice, TextureClass shape);

	void UpdateAbsoluteSize();
	void ensureResource();
	void ensureRenderTargetView();
	void ensureBuffer();
	ID3D11View *getView();
	void initForRenderTarget(IRenderSurface *renderSurface, uint32_t format, ID3D11Texture2D *texture);

	void resFree() override;
	void resFill(void* src, uint32_t size, uint32_t offset) override;
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
	ID3D11DepthStencilView *createDepthStencilView(); // read-only depth
	void createShaderResourceView_internal(ID3D11Resource *, ID3D11ShaderResourceView **);
};

}
