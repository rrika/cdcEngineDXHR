#pragma once
#include "IMaterial.h"
#include <cstdint>

namespace cdc {

struct MaterialBlob {
	// TODO
};

class CommonMaterial : public IMaterial {
	uint32_t dword4;
	uint32_t dword8;
	MaterialBlob *materialBlob = nullptr;
public:
	void method_04() override;
	void method_0C() override;
	void method_10() override;
	~CommonMaterial() = default;
	virtual void method_18() = 0;
};

}
