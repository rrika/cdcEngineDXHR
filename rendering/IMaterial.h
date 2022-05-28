#pragma once

namespace cdc {

struct MaterialBlob;

class IMaterial {
public:
	virtual void load(MaterialBlob*) = 0;
	virtual void method_04() = 0;
	virtual void method_08() = 0;
	virtual void method_0C() = 0;
	virtual void method_10() = 0;
	virtual ~IMaterial() = default;
};

}
