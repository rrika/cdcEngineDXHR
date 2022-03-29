#pragma once

namespace cdc {

class IRenderSurface {
	virtual void getRenderTexture() = 0;
	virtual void getWidth() = 0;
	virtual void getHeight() = 0;
	virtual void method_0C() = 0;
	virtual void method_10() = 0;
	virtual void method_14();
	virtual void method_18();
	virtual ~IRenderSurface() = default;
};

}
