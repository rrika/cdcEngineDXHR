#include "PPManager.h" // for PPRTs
#include "PPTexture.h"
#include "rendering/CommonRenderDevice.h"
#include "rendering/surfaces/CommonRenderTarget.h" 
#include "rendering/surfaces/CommonDepthBuffer.h" 

void PPTexture::init(dtp::PPTextureBlob *newBlob) {
	blob = *newBlob;

	if (blob.dword4) {
		// TODO
	} else {

		if (blob.dword8 == 0)
			dword24 = 1;
		else if (blob.dword8 == 1)
			dword24 = 2;
		else if (blob.dword8 == 2)
			dword24 = 3;
	}
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
