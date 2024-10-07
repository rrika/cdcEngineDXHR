#pragma once
#include "../IRenderDrawable.h"

namespace cdc {

class PCDX11RenderDevice;
class PCDX11RenderTarget;
class PCDX11BaseTexture;

class PCDX11MLAADrawable :
	public IRenderDrawable
{
	uint32_t flags; // C
	PCDX11RenderDevice *renderDevice; // 10
	PCDX11RenderTarget *renderTarget; // 14
	PCDX11BaseTexture *texture; // 18

public:
	PCDX11MLAADrawable(
		PCDX11RenderDevice *renderDevice,
		PCDX11BaseTexture *texture,
		PCDX11RenderTarget *renderTarget,
		uint32_t flags,
		float sortZ
	) :
		flags(flags),
		renderDevice(renderDevice),
		renderTarget(renderTarget),
		texture(texture)
	{
		this->sortZ = sortZ;
	}

	virtual void draw(uint32_t funcSetIndex, IRenderDrawable *other);
	virtual uint32_t compare(uint32_t funcSetIndex, IRenderDrawable *other);
};

}
