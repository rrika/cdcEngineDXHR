#include "GlobalParamManager.h"

void ApplyGlobalParams(cdc::Vector *globalParams, GlobalParamInit const *init) {
	for (uint32_t i=0; i<11; i++) {
		cdc::Vector vec;
		auto sel = init->selector[i];
		if (sel == 0)
			vec = {0.f, 0.f, 0.f, 0.f};
		else if (sel >= 1 && sel < 9)
			vec = init->packedVectors[sel-1].unpack();
		else if (sel >= 9)
			vec = init->plainVectors[sel-9];
		globalParams[i] = vec;
	}
}
