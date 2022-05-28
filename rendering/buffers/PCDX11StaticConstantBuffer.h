#pragma once
#include "PCDX11ConstantBuffer.h"
#include "../PCDX11AsyncCreateResource.h"
#include "../PCDX11DeviceManager.h"
#include <d3d11.h>

namespace cdc {

class PCDX11StaticConstantBuffer :
	public PCDX11ConstantBuffer,
	public PCDX11AsyncCreateResource
{
public:
	ID3D11Buffer *getBuffer() override {
		if (m_keepWaiting)
			awaitResource();
		return buffer;
	}
	void asyncCreate() override {
		D3D11_BUFFER_DESC desc;
		desc.ByteWidth = size;
		desc.MiscFlags = 0;
		desc.StructureByteStride = 0;
		desc.CPUAccessFlags = 0;
		desc.Usage = D3D11_USAGE_IMMUTABLE;
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		D3D11_SUBRESOURCE_DATA subResourceData = { data, 0, 0 };
		deviceManager->getD3DDevice()->CreateBuffer(&desc, &subResourceData, &buffer);
	}
};

}
