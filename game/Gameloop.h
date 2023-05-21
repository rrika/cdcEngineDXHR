#pragma once
#include "game/dtp/objecttypes/globaldatabase.h"

extern GlobalDatabase *globalDatabase;

namespace Gameloop {

void InitiateLevelLoad(const char *baseAreaName, void *gameTracker);

}
