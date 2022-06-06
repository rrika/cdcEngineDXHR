#pragma once
#include <stdint.h>

namespace cdc {

class Heap {
public:
	virtual ~Heap() = default;
	virtual void method_04() = 0;
	virtual void method_08() = 0;
};

}
