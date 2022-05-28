#include "../PCDX11DeviceManager.h"
#include "PCDX11BitmapTexture.h"
#include <d3d11.h>

namespace cdc {

ID3D11Resource *PCDX11BitmapTexture::getTextureResource() {
	return d3dTexture128;
}

ID3D11ShaderResourceView *PCDX11BitmapTexture::createShaderResourceView() {
	if (baseTexMethodC())
		baseTexMethod10();

	if (d3dResourceView12C != nullptr || d3dTexture128 == nullptr)
		return d3dResourceView12C;

	D3D11_SHADER_RESOURCE_VIEW_DESC desc {};

	if (shape == 2) {
		desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE3D;
		desc.Texture2D.MostDetailedMip = 0;
		desc.Texture2D.MipLevels = -1;
	} else if (shape == 3) {
		desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
		desc.TextureCube.MostDetailedMip = 0;
		desc.TextureCube.MipLevels = -1;
	} else {
		desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		desc.Texture3D.MostDetailedMip = 0;
		desc.Texture3D.MipLevels = -1;
	}

	deviceManager->getD3DDevice()->CreateShaderResourceView(d3dTexture128, &desc, &d3dResourceView12C);

	return d3dResourceView12C;
}

ID3D11UnorderedAccessView *PCDX11BitmapTexture::createUnorderedAccessView() {
	if (baseTexMethodC())
		baseTexMethod10();

	if (d3dAccessView130 != nullptr || d3dTexture128 == nullptr)
		return d3dAccessView130;

	D3D11_UNORDERED_ACCESS_VIEW_DESC desc {};
	desc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
	desc.Format = (DXGI_FORMAT)textureFormat;

	deviceManager->getD3DDevice()->CreateUnorderedAccessView(d3dTexture128, &desc, &d3dAccessView130);

	return d3dAccessView130;
}

}
