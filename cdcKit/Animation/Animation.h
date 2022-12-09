#pragma once

class Instance;

namespace cdc {

class AnimComponent;
class AnimProcessor;

struct BlendProcessor { // line 97
	AnimComponent *animComponent0;
	uint32_t dword4;
	uint32_t dword8;
	float floatC;
};

class AnimComponent { // line 166
	BlendProcessor blendprocessor; // 0
	AnimProcessor *processor; // 10
	uint32_t numProcessors; // 14
	Instance *instance; // 18
	uint32_t model; // 1C
	uint32_t faceFxComponent; // 20
};

}
