#pragma once
#include "../IRenderDrawable.h"

namespace cdc {

class PCDX11BaseTexture;
class PCDX11RenderDevice;
class PCDX11RenderTarget;

class PCDX11FastBlurDrawable :
	public IRenderDrawable
{
	bool isHorizontalPass;
	// uint8_t byteD;
	// uint32_t dword10;
	bool weighted;
	PCDX11RenderDevice *m_renderDevice; // 18
	PCDX11RenderTarget *m_renderTarget; // 1C
	PCDX11BaseTexture *m_texture; // 20
public:
	PCDX11FastBlurDrawable(
		PCDX11RenderDevice *renderDevice,
		PCDX11BaseTexture *texture,
		PCDX11RenderTarget *renderTarget,
		bool isHorizontalPass,
		bool weighted)
	:
		isHorizontalPass(isHorizontalPass),
		weighted(weighted),
		m_renderDevice(renderDevice),
		m_renderTarget(renderTarget),
		m_texture(texture)
	{}

	virtual void draw(uint32_t funcSetIndex, IRenderDrawable *other);
	virtual uint32_t compare(uint32_t funcSetIndex, IRenderDrawable *other);
};

}
