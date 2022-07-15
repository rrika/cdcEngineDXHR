#pragma once
#include <cstdint>
#include "IMaterial.h"

namespace cdc {

struct MaterialBlob;

class CommonMaterial : public IMaterial {
	uint32_t dword4;
	uint32_t dword8;
public:
	MaterialBlob *materialBlob = nullptr;
public:
	void method_04() override;
	void method_0C() override;
	void method_10() override;
	~CommonMaterial() = default;
	virtual void method_18() = 0;
};

}
