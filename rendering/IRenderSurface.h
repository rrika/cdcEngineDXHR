#pragma once

namespace cdc {

class IRenderSurface {
public:
	virtual void getRenderTexture() = 0;
	virtual void getWidth() = 0;
	virtual void getHeight() = 0;
	virtual void method_0C() = 0;
	virtual void method_10() = 0;
	virtual void method_14() = 0;
	virtual bool method_18() { return false; };
	virtual void method_1C() {};
	virtual ~IRenderSurface() = default;
};

}
