#pragma once
#include "cdc/dtp/objectproperties/intro.h"

class BlobXXX;
class UnitBlobSignal;

namespace dtp {

struct ADMD {
	uint32_t numLightTable4;
	uint32_t lightTable4;
	uint32_t dword8;
	uint32_t numLightTable10;
	uint32_t lightTable10;
	uint32_t numObjects;
	Intro *objects;
	uint32_t smallerObjects;
	uint32_t numSmallerObjects;
	uint32_t dword24;
	uint32_t dword28;
	uint32_t dword2C;
	uint32_t dword30;
	uint32_t dword34;
	uint32_t dword38;
	uint32_t dword3C;
	uint32_t dword40;
	uint32_t dword44;
	uint32_t num_signals;
	UnitBlobSignal *signals;
	uint32_t dword50;
	uint32_t dword54;
	uint32_t dword58;
	uint32_t dword5C;
	uint32_t dword60;
	uint32_t dword64;
	uint32_t dword68;
	uint32_t dword6C;
	uint32_t dword70;
	uint32_t dword74;
	uint32_t dword78;
	uint32_t dword7C;
	uint32_t dword80;
	uint32_t dword84;
	uint32_t dword88;
	uint32_t dword8C;
	uint32_t dword90;
	uint32_t dword94;
	uint32_t dword98;
	uint32_t dword9C;
	uint32_t dwordA0;
	uint32_t numXXX;
	BlobXXX *imfxxx;
	uint32_t dwordAC;
};

}
