#pragma once
#include <d3d11_1.h>
#include "CommonIndexBuffer.h"
#include "../types.h"

namespace cdc {

class PCDX11IndexBuffer : public CommonIndexBuffer {
public:
	virtual uint getStartIndex();
	virtual ID3D11Buffer *getD3DBuffer();
	virtual void method_0C();
	virtual void method_10();
};

}
