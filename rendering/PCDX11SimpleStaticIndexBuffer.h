#pragma once
#include "PCDX11IndexBuffer.h"
#include "PCDX11DeviceManager.h"

namespace cdc {

class PCDX11SimpleStaticIndexBuffer :
	public PCDX11IndexBuffer
{
public:
	PCDX11SimpleStaticIndexBuffer(uint32_t count, void *data) :
		count(count)
	{
		D3D11_BUFFER_DESC vertexBufferDesc = {};
		vertexBufferDesc.ByteWidth = 2 * count;
		vertexBufferDesc.Usage     = D3D11_USAGE_IMMUTABLE;
		vertexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		D3D11_SUBRESOURCE_DATA subresourceData = { data };
		deviceManager->getD3DDevice()->CreateBuffer(&vertexBufferDesc, &subresourceData, &buffer);
	}

	uint32_t count;
	ID3D11Buffer *buffer;

	uint getStartIndex() override { return 0; };
	ID3D11Buffer *getD3DBuffer() override { return buffer; }; // getDword8
	uint32_t getCount() override { return count; }; // getDword4
	void method_10() override {};
};

}
