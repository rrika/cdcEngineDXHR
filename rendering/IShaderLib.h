#pragma once
#include <cstdint>

namespace cdc {

class IShaderLib {
public:
	virtual ~IShaderLib() = default;
	virtual void fill(char *buffer, uint32_t offset, uint32_t size, bool done) = 0;
	// TODO: method8
};

}
