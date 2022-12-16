#pragma once
#include "cdcSound.h"
#include "cdcSys/RCObject.h"

namespace cdc {

class SoundOwner : public RCObject, protected SoundTypes {
public:
	Controls controls;
	Controls3d controls3d;
};

}
