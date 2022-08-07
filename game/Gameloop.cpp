#include "Gameloop.h"
#include "../world/stream.h"

namespace Gameloop {

void InitiateLevelLoad(const char *baseAreaName, void *gameTracker) {
	// TODO
	STREAM_LevelLoadAndInit(baseAreaName);
	// TODO
}

}
