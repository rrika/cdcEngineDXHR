#pragma once
#include "cdcSound.h"
#include "cdcSys/RCObject.h"

namespace cdc {

class SoundPlex;

class SoundOwner : public RCObject, protected SoundTypes {
public:
	SoundPlex *plex; // 8
	Controls controls; // 14
	Controls3d controls3d; // 50
};

using SoundHandle = Handle<SoundOwner>;

}
