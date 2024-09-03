#include "PPManager.h" // for PPRTs
#include "PPTexture.h"
#include "rendering/CommonRenderDevice.h"
#include "rendering/surfaces/CommonRenderTarget.h" 
#include "rendering/surfaces/CommonDepthBuffer.h" 

void PPTexture::init(dtp::PPTextureBlob *newBlob) {
	blob = *newBlob;

	if (blob.dword4) {
		
		uint32_t flags = 1;
		uint32_t format = 0;

		if (blob.dword8 == 1)
			flags |= 8;
		if (blob.byte18)
			flags |= 4;

		if (blob.dword14 == 0)
			format = 21;  // -> DXGI_FORMAT_R8G8B8A8_UNORM
		else if (blob.dword14 == 1)
			format = 112; // -> DXGI_FORMAT_R16G16_FLOAT
		else if (blob.dword14 == 2)
			format = 114; // -> DXGI_FORMAT_R32_FLOAT

		renderTarget = static_cast<cdc::CommonRenderTarget*>(
			cdc::g_renderDevice->createRenderTarget(
				blob.width,
				blob.height,
				flags, format, format, cdc::kTextureClassUnknown));

	} else {

		if (blob.dword8 == 0)
			dword24 = 1;
		else if (blob.dword8 == 1)
			dword24 = 2;
		else if (blob.dword8 == 2)
			dword24 = 3;
	}
}

cdc::CommonRenderTarget *PPTexture::getRenderTarget() {
	return blob.dword4 ? renderTarget : nullptr;
}

cdc::TextureMap *PPTexture::getRenderTexture(PPRTs *rts) {
	if (blob.dword4 == 0) { // use PPRTs

		if (blob.dword8 == 0)
			return rts->rt0->getRenderTexture();

		else if (blob.dword8 == 1)
			return rts->db->getRenderTexture();

		else if (blob.dword8 == 2)
			return rts->rt8->getRenderTexture();

		else
			return nullptr;

	} else if (blob.dword4 == 1) // use own RT
		return renderTarget->getRenderTexture();

	else
		return nullptr;
}

void PPTexture::freeRenderTarget() {
	if (renderTarget) { // HACK
		renderTarget->EvictNow();
		delete renderTarget;
		renderTarget = nullptr;
	}
	// if (!dword24)
	// 	renderTarget->method_14();
}
