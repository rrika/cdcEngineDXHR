#pragma once
#include <cstdint>

namespace cdc {

class IMatrixState {
public:
	virtual void resize(uint32_t) = 0;
	// virtual void method_04() = 0;
	// virtual void method_08() = 0;
	// virtual void method_0C() = 0;
	// virtual void method_10() = 0;
	// virtual void method_14() = 0;
	// virtual void method_18() = 0;
	// virtual void method_1C() = 0;
	// virtual void method_20() = 0;
	// virtual void method_24() = 0;
	// virtual void method_28() = 0;
	// virtual void method_2C() = 0;
	// virtual void method_30() = 0;
	// virtual inline uint8_t method_34() { return 0; };
	virtual ~IMatrixState() = default; // 38
};

}
