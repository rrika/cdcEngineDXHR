#pragma once
#include <cstdint>

namespace cdc {

class ResolveObject;

class Resolve {
public:
	static void Create();
	static void Destroy();
	static int32_t Release(ResolveObject*&);
	static int32_t GetRefCount(ResolveObject*);
};

}
