#pragma once
#include <cstdint>

namespace cdc {

class IRenderDrawable {
public:
	virtual void renderDrawable0() = 0;
	virtual uint32_t renderDrawable4() = 0;
};

}
