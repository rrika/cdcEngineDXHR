#pragma once

namespace cdc {

class IMaterial {
public:
	virtual void method_00() = 0;
	virtual void method_04() = 0;
	virtual void method_08() = 0;
	virtual void method_0C() = 0;
	virtual void method_10() = 0;
	virtual ~IMaterial() = 0;
};

}
