#include "Gameloop.h"
#include "cdcWorld/stream.h"

GlobalDatabase *globalDatabase = nullptr;

namespace Gameloop {

void InitiateLevelLoad(const char *baseAreaName, void *gameTracker) {
	// TODO
	STREAM_LevelLoadAndInit(baseAreaName);
	// TODO
}

}
