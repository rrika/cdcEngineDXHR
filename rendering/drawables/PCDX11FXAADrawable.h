#pragma once
#include "../IRenderDrawable.h"

namespace cdc {

class PCDX11RenderDevice;
class PCDX11RenderTarget;
class PCDX11Texture;

class PCDX11FXAADrawable :
	public IRenderDrawable
{
	uint32_t flags;
	PCDX11RenderDevice *renderDevice;
	uint32_t quality;
	PCDX11RenderTarget *renderTarget;
	PCDX11Texture *texture;

public:
	PCDX11FXAADrawable(
		PCDX11RenderDevice *renderDevice,
		uint32_t quality,
		PCDX11Texture *texture,
		PCDX11RenderTarget *renderTarget,
		uint32_t flags,
		float sortZ
	) :
		flags(flags),
		renderDevice(renderDevice),
		quality(quality),
		renderTarget(renderTarget),
		texture(texture)
	{
		this->sortZ = sortZ;
	}

	virtual void draw(uint32_t funcSetIndex, IRenderDrawable *other);
	virtual uint32_t compare(uint32_t funcSetIndex, IRenderDrawable *other);
};

}
