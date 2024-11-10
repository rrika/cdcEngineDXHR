#include <cstdio>
#include <cstring>
#include <iterator>
#include "config.h"
#include "SceneLayer.h" // for StreamingCallback
#include "stream.h"
#include "StreamUnit.h"
#include "cdcFile/FileHelpers.h"
#include "cdcFile/FileSystem.h"
#include "cdcGameShell/cdcGameShell.h" // for LOAD_UnitFileName
#include "cdcObjects/ObjectManager.h" // for readAndParseObjectList
#include "cdcResource/ResolveObject.h"
#include "cdcSound/sfxmarker.h"
#include "cdcWorld/Instance.h"
#include "cdc/dtp/objectproperties/sfxmarker.h"
#include "game/dtp/objecttypes/globalplayerinfo.h"

#if ENABLE_IMGUI
#include "../game/Gameloop.h"
#include "../imgui/imgui.h"
#endif

using namespace cdc;

StreamUnit StreamTracker[80] = {};

extern char buildType[16];
extern GlobalPlayerInfo *globalPlayerInfo;

struct ObjListEntry {
	char name[128];
	uint16_t *data;
};

struct ObjList {
	uint32_t count;
	ObjListEntry entries[];
};

ObjList *objList;

const char *unitNameById(uint32_t id) {
	if (id < objList->count)
		return objList->entries[id].name;
	else
		return nullptr;
}

#if ENABLE_IMGUI
static void buildUnitsUI(bool main, bool scn) {
	for (uint32_t i = 0; i < objList->count; i++) {
		char *name = objList->entries[i].name;
		if (name[0] == 0)
			continue;
		bool isScn = name[0] == 's' && name[1] == '_';
		if ((isScn ? scn : main) == false)
			continue;
		// ImGui::Text("%s", objList->entries[i].name);
		if (ImGui::SmallButton(objList->entries[i].name)) {
			Gameloop::InitiateLevelLoad(objList->entries[i].name, nullptr);
		}
	}
}

void buildUnitsUI() {
	static bool splitScenarios = true;
	ImGui::Checkbox("Show scenarios separately", &splitScenarios);
	if (splitScenarios) {
		buildUnitsUI(true, false);
		ImGui::Separator();
		buildUnitsUI(false, true);
	} else {
		buildUnitsUI(true, true);
	}
}
#else
void buildUnitsUI() {}
#endif

static StreamingCallback *createObjectStreamingCallback();

void STREAM_Init() { // 582
	// TODO

	// read object names
	readAndParseObjectList();

	// read unit names
	char objListPath[300];
	sprintf(objListPath, "%s\\objlist.dat", buildType);
	if (getDefaultFileSystem()->getSize(objListPath)) {
		objList = (ObjList*) FSHelper_ReadFile(objListPath);
		uint32_t count = objList->count;
		ObjListEntry *entry = objList->entries;
		uint16_t *data = (uint16_t*)&objList->entries[objList->count];
		while (count--) {
			if (entry->name[0] != '\0') {
				entry->data = data;
				data += 1 + *data;
			}
			entry++;
		}
	}

	// TODO
	static StreamingCallback *cb = createObjectStreamingCallback();
	SceneLayer::AddStreamingCallback(cb);
}

StreamUnit *STREAM_GetStreamUnitWithID(int32_t id) { // 1170
	for (auto &unit : StreamTracker)
		if ((unit.used == 1 || unit.used == 2) && unit.StreamUnitID == id)
			return &unit;
	return nullptr;
}

StreamUnit *STREAM_GetAndInitStreamUnitWithID(const char *name, int32_t id) { // couldn't confirm real name
	StreamUnit *unit = StreamTracker;
	while (unit < std::end(StreamTracker) && unit->used)
		unit++;
	if (unit == std::end(StreamTracker))
		return nullptr;

	unit->used = 1;
	strcpy((char*)unit->name, name);
	unit->StreamUnitID = id; // might be a dummy ID, put proper ID in STREAM_LoadLevelReturn
	// unit->wordA = 0;
	// unit->word240 = 0;
	// unit->dword4 = 0;
	// unit->dwordB4 = 0;
	// unit->byte9 = 0;
	// unit->dword9C = 0;
	unit->coreUnit = nullptr;
	unit->resolveObject_streamGroup = nullptr;
	// unit->byte94 = 0;

	// TODO
	return unit;
}

void STREAM_FinishLoad(StreamUnit *unit) { // 1658
	printf("unit->level=%p\n", unit->level);
	dtp::ADMD *admd = unit->level->admdData;
	for (uint32_t i=0; i<admd->m_SfxMarkerCount; i++)
		SFXMARKER_Create(admd->m_ppSfxMarkers[i]);

	// TODO
	SceneLayer::PreStreamIn(unit);
	// TODO
	SceneLayer::PostStreamIn(unit); // this calls UnitLoaded()
	// TODO
}

void STREAM_LoadLevelReturn(void *loadData, void *data, void *data2, ResolveObject *resolveObj) { // 1922
	StreamUnit *unit = (StreamUnit*)data2;
	unit->StreamUnitID = ((Level*)loadData)->streamUnitID;
	// TODO
	STREAM_FinishLoad(unit);
	// TODO
}

void STREAM_StreamLoadLevelReturn(void *loadData, void *data, void *data2, ResolveObject *resolveObj) { // 1971
	StreamUnit *unit = (StreamUnit*)data2;
	unit->StreamUnitID = ((Level*)loadData)->streamUnitID;
	// TODO
	STREAM_FinishLoad(unit);
	// TODO
}

StreamUnit *STREAM_LoadLevel(const char *baseAreaName, StreamUnitPortal *streamPortal, bool loadObjects) { // 2085
	// TODO
	if (true) {
		// TODO
		int32_t i = -1;
		if (streamPortal)
			i = streamPortal->word84;
		StreamUnit *unit = STREAM_GetAndInitStreamUnitWithID(baseAreaName, i);
		if (unit) {
			char filename[256];
			GameShell::LOAD_UnitFileName(filename, baseAreaName);
			ResolveObject *resolveObject;
			if (streamPortal) {
				resolveObject = ResolveObject::create(
					filename,
					&STREAM_StreamLoadLevelReturn,
					0,
					unit,
					(void**)&unit->level, // put pointer to blob here
					0,
					0,
					0,
					cdc::FileRequest::NORMAL
				);
			} else {
				resolveObject = ResolveObject::create(
					filename,
					&STREAM_LoadLevelReturn,
					0,
					unit,
					(void**)&unit->level, // put pointer to blob here
					0,
					0,
					0,
					cdc::FileRequest::NORMAL
				);
			}
		}
		return unit;
	}
}

// only called from scripting system
void STREAM_LoadLevel(const char *baseAreaName) { // 2171
	// TODO
}

StreamUnit *STREAM_LevelLoadAndInit(const char *baseAreaName) { // 2855
	// TODO

	StreamUnit *streamUnit = STREAM_LoadLevel(baseAreaName, nullptr, true);

	// TODO

	// HACK: exact way of waiting for the filesystem differs
	cdc::getDefaultFileSystem()->processAll();
	// after waiting for fs to finish the following will have happened:
	//   STREAM_FinishLoad
	//     SceneLayer::PreStreamIn
	//     SceneLayer::PostStreamIn
	//       ObjectStreamingCallback::UnitLoaded

	// TODO

	// HACK: substitute player.drm for jensen_trenchcoat.drm or such
	dtp::ADMD *admd = streamUnit->level->admdData;
	for (uint32_t i = 0; i < admd->numObjects; i++) {
		auto *intro = &admd->objects[i];
		if (intro->objectListIndex == 0x12) { // player.drm
			char *extraData = (char*)intro->extraData1;
			if (*(uint32_t*)extraData == 0x40B) {
				uint32_t jensenIndex = *(uint32_t*)(extraData+0x2C);
				uint32_t total = globalPlayerInfo->numJensens;
				uint16_t newIndex = globalPlayerInfo->jensens[jensenIndex];
				if (newIndex) {
					intro->objectListIndex = newIndex; // jensen_trenchcoat.drm, jensen_dlc.drm, etc.
					requestObjectNormal(newIndex);
				}
			}
		}
	}
	cdc::getDefaultFileSystem()->processAll();

	// HACK: exact way of instantiating differs
	for (uint32_t i = 0; i < admd->numObjects; i++) {
		Instance::IntroduceInstance(&admd->objects[i], streamUnit->StreamUnitID, /*force=*/ false);
	}

	// TODO
	return streamUnit;
}

class ObjectStreamingCallback : public StreamingCallback { // 4088
public:
	void UnitLoaded(StreamUnit *unit) override {
		// TODO
		printf("ObjectStreamingCallback::UnitLoaded(%p)\n", unit);
		// TODO
		dtp::ADMD *admd = unit->level->admdData;
		for (uint32_t i = 0; i < admd->numObjects; i++) {
			dtp::Intro *intro = &admd->objects[i];
			auto id = intro->objectListIndex;
			printf("  obj %d/%d %d %s %02x %02x\n", i, admd->numObjects, 
				id, id ? g_objectManager->objectList->entries[id-1].name : "0",
				intro->field41, intro->flags);
			if (intro->objectListIndex == 0) // HACK
				continue;
			requestObjectNormal(intro->objectListIndex);
		}
	}
	void UnitDumped(StreamUnit*) override {
		// TODO
	}
	void StreamGroupLoaded(cdc::CellStreamGroupData*) override {
		// TODO
	}
	void StreamGroupDumped(cdc::CellStreamGroupData*) override {
		// TODO
	}
};

// workaround for weird declaration order
static StreamingCallback *createObjectStreamingCallback() {
	return new ObjectStreamingCallback();
}
