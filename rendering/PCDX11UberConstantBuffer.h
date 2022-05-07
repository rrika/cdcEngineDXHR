#pragma once
#include "PCDX11ConstantBuffer.h"
#include <cstdio>
#include <cstring>
#include <d3d11.h>

namespace cdc {

class PCDX11UberConstantBuffer : public PCDX11ConstantBuffer {
public:
	PCDX11UberConstantBuffer(uint32_t rows)
		: PCDX11ConstantBuffer(rows)
	{
		data = new char[16 * rows];
	}

	void ensureBuffer() {
		if (buffer == nullptr) {
			D3D11_BUFFER_DESC desc = {};
			desc.ByteWidth = size;
			desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			desc.Usage = D3D11_USAGE_DYNAMIC;
			desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			deviceManager->getD3DDevice()->CreateBuffer(&desc, nullptr, &buffer);
		}
	}

	void syncBuffer(ID3D11DeviceContext *deviceContext) {
		ensureBuffer();

		D3D11_MAPPED_SUBRESOURCE mappedSubresource;
		deviceContext->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubresource);
		memcpy(mappedSubresource.pData, data, size);
		deviceContext->Unmap(buffer, 0);
	}
};

}
