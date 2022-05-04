#pragma once
#include "PCDX11VertexBuffer.h"

namespace cdc {

class PCDX11StaticVertexBuffer :
	public PCDX11VertexBuffer
	// public PCDX11DeferredUpdate
{
public:
	PCDX11StaticVertexBuffer(ID3D11Buffer* buffer, uint32_t stride) : // hack
		stride(stride),
		buffer(buffer)
	{}

	uint32_t stride;
	uint32_t dword10;
	uint32_t baseVertex;
	ID3D11Buffer *buffer;

	uint32_t getBaseVertex() { return baseVertex; }
	ID3D11Buffer *getD3DBuffer() { return buffer; }
	uint32_t method0C() { return dword10; }
	uint32_t getStride() { return stride; }
	void method14() {}
};

}
