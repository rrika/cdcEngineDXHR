#pragma once
#include "cdcSound.h"
#include "cdcSys/RCObject.h"

namespace cdc {

class SoundPlex;
class Vector;

class SoundOwner : public RCObject, protected SoundTypes {
public:
	SoundPlex *plex; // 8
	Controls controls; // 14
	Controls3d controls3d = {{.playbackType=0}}; // 50

	void SetPosition(Vector pos);
	void End(EndType endType);
};

using SoundHandle = Handle<SoundOwner>;

}
