#include <cstdio>
#include "stream.h"
#include "StreamUnit.h"
#include "../drm/ResolveObject.h"
#include "../game/Main.h" // for buildDRMPath

StreamUnit StreamTracker[80];

StreamUnit *STREAM_GetStreamUnitWithID(int32_t id) { // 1170
	// HACK
	return &StreamTracker[0];
}

StreamUnit *STREAM_GetAndInitStreamUnitWithID(int32_t id) { // couldn't confirm real name
	auto *unit = STREAM_GetStreamUnitWithID(id);
	// TODO
	return unit;
}

void STREAM_FinishLoad(StreamUnit *unit) { // 1658
	printf("unit->level=%p\n", unit->level);
}

void STREAM_LoadLevelReturn(void *loadData, void *data, void *data2, cdc::ResolveObject *resolveObj) { // 1922
	StreamUnit *unit = (StreamUnit*)data2;
	// TODO
	STREAM_FinishLoad(unit);
	// TODO
}

void STREAM_StreamLoadLevelReturn(void *loadData, void *data, void *data2, cdc::ResolveObject *resolveObj) { // 1971
	StreamUnit *unit = (StreamUnit*)data2;
	// TODO
	STREAM_FinishLoad(unit);
	// TODO
}

void STREAM_LoadLevel(const char *baseAreaName, StreamUnitPortal *streamPortal, bool loadObjects) { // 2085
	// TODO
	if (true) {
		// TODO
		int32_t i = -1;
		if (streamPortal)
			i = streamPortal->word84;
		StreamUnit *unit = STREAM_GetAndInitStreamUnitWithID(i);
		if (unit) {
			char filename[256];
			buildDRMPath(filename, baseAreaName);
			cdc::ResolveObject *resolveObject;
			if (streamPortal) {
				resolveObject = cdc::ResolveObject::create(
					filename,
					&STREAM_StreamLoadLevelReturn,
					0,
					unit,
					(void**)&unit->level, // put pointer to blob here
					0,
					0,
					0,
					3 // normal priority
				);
			} else {
				resolveObject = cdc::ResolveObject::create(
					filename,
					&STREAM_LoadLevelReturn,
					0,
					unit,
					(void**)&unit->level, // put pointer to blob here
					0,
					0,
					0,
					3 // normal priority
				);
			}
		}
	}
}

// only called from scripting system
void STREAM_LoadLevel(const char *baseAreaName) { // 2171
	// TODO
}

StreamUnit *STREAM_LevelLoadAndInit(const char *baseAreaName) { // 2855
	// TODO
	STREAM_LoadLevel(baseAreaName, nullptr, true);
	// TODO
	return nullptr;
}
