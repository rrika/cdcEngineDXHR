#include "cdcMath/Math.h" // remove me
#include "SoundOwner.h"
#include "SoundPlex.h"

namespace cdc {

void SoundOwner::SetPosition(Vector pos) {
	controls3d.playbackType |= 0x100;
	controls3d.position[0] = pos.x;
	controls3d.position[1] = pos.y;
	controls3d.position[2] = pos.z;
	controls3d.position[3] = pos.w;
}

void SoundOwner::End(EndType endType) {
	if (!plex) // HACK
		return; // this shouldn't happen, find out why it does
	plex->End(endType);
}

}
