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
#include "cdcResource/Resolve.h"
#include "cdcResource/ResolveObject.h"
#include "cdcSound/sfxmarker.h"
#include "cdcWorld/Instance.h"
#include "cdcWorld/InstanceManager.h"
#include "cdc/dtp/objectproperties/sfxmarker.h"
#include "game/dtp/objecttypes/globalplayerinfo.h"

#if ENABLE_IMGUI
#include "cdcMath/VectorInlines.h"
#include "cdcScene/SceneCell.h"
#include "../game/Gameloop.h"
#include "../imgui/imgui.h"
#include "../UIActions.h"
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

static bool isPlayer(uint16_t objectListIndex) {
	if (objectListIndex == 0x12) // player.drm
		return true;
	uint32_t numJensens = globalPlayerInfo->numJensens;
	for (auto i=0u; i<numJensens; i++)
		if (globalPlayerInfo->jensens[i] == objectListIndex)
			return true;
	return false;
}

static void recommendedPostLoadActions(UIActions& uiact, ObjListEntry& entry) {
	StreamUnit *pUnit = nullptr;
	for (auto &unit : StreamTracker)
		if (unit.used != 0 && strcmp((char*)unit.name, entry.name) == 0) {
			pUnit = &unit;
			break;
		}
	if (!pUnit) {
		ImGui::Text("strange, unit not found");
		return;
	}
	StreamUnit& unit = *pUnit;

	dtp::ADMD *admd = unit.level->admdData;
	Vector3 cameraPos {uiact.getCameraPosition()};

	// [Load static objects (IMF)]
	uint32_t numIMFRefs = admd->numIMFRefs;
	ImGui::BeginDisabled(unit.imfShowRange[1] == numIMFRefs);
	if (ImGui::SmallButton("Load static objects (IMF)"))
		unit.imfShowRange[1] = numIMFRefs;
	ImGui::EndDisabled();

	// Spawnpoint N
	ImGui::SeparatorText("Spawn points");
	for (uint32_t i = 0; i < admd->numObjects; i++) {
		auto *intro = &admd->objects[i];
		if (isPlayer(intro->objectListIndex)) {
			char label[100]; sprintf(label, "%s %i", cdc::objectName(intro->objectListIndex), i);
			Vector spawnPos {
				intro->position[0],
				intro->position[1],
				intro->position[2] + 580.f // approx. eye height
			};
			bool match = // someone should make an operator== overload
				cameraPos.x == spawnPos.x &&
				cameraPos.y == spawnPos.y &&
				cameraPos.z == spawnPos.z;
			if (ImGui::MenuItem(label, nullptr, match)) {
				uiact.camTeleport(spawnPos, intro->rotation[2]);
			}
		}
	}

	// Cell X
	ImGui::SeparatorText("Cells");
	uint32_t limit = 30;
	if (cdc::CellGroupData *cellGroupData = unit.level->pSceneData) {
		auto *playerCell = static_cast<cdc::SceneCell*>(
			unit.sceneCellGroup->GetCellFromPoint(cameraPos, false));
		uint32_t numCells = cellGroupData->header->numTotalCells;
		for (uint32_t i=0; i < numCells; i++) {
			cdc::CellData *cell = cellGroupData->cells[i];
			bool insideCell = playerCell && playerCell->m_pCellData == cell;
			if (ImGui::MenuItem(cell->pHeader->name, nullptr, insideCell)) {
				float *center = cell->pHeader->vec10;
				Vector p { center[0], center[1], center[2] };
				uiact.camTeleport(p);
			}
			if (limit-- == 0)
				break;
		}
	}
}

static ImGuiTextFilter unitFilter("-preorder,-global_light,");

static void buildUnitsUI(UIActions& uiact, bool main, bool scn) {
	for (uint32_t i = 0; i < objList->count; i++) {
		char *name = objList->entries[i].name;
		if (name[0] == 0)
			continue;
		bool isScn = name[0] == 's' && name[1] == '_';
		if ((isScn ? scn : main) == false)
			continue;
		if (!unitFilter.PassFilter(name))
			continue;
		// ImGui::Text("%s", name);
		char filename[256];
		GameShell::LOAD_UnitFileName(filename, name);
		ImGui::BeginDisabled(getDefaultFileSystem()->FileExists(filename) == false);
		if (ImGui::SmallButton(name)) {
			Gameloop::InitiateLevelLoad(name, nullptr);
			ImGui::OpenPopup(name);
		}
		if (ImGui::BeginPopup(name)) {
			recommendedPostLoadActions(uiact, objList->entries[i]);
			ImGui::EndPopup();
		}
		ImGui::EndDisabled();
	}
}

void buildUnitsUI(UIActions& uiact) {
	static bool splitScenarios = true;
	ImGui::Checkbox("Show scenarios separately", &splitScenarios);
	ImGui::SetNextItemShortcut(ImGuiMod_Ctrl | ImGuiKey_L);
	unitFilter.Draw();
	ImGui::SetItemDefaultFocus();
	if (ImGui::IsWindowAppearing())
		ImGui::SetKeyboardFocusHere(-1);
	if (splitScenarios) {
		buildUnitsUI(uiact, true, false);
		ImGui::Separator();
		buildUnitsUI(uiact, false, true);
	} else {
		buildUnitsUI(uiact, true, true);
	}
}
#else
void buildUnitsUI() {}
#endif

static StreamingCallback *createObjectStreamingCallback();

void STREAM_Init() { // line 582
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

void STREAM_FreeResources() { // line 976
	for (auto &unit : StreamTracker) {
		if (unit.used == 4) {
			// if (unit.sceneCellGroup)
			//	SceneLayer::StreamUnitDataDumped(unit);
			// StreamDeferredManager::getInstance()->fun1(unit);
			// StreamDeferredManager::getInstance()->fun2(unit);

			Resolve::Release(unit.resolveObject);
			unit.level = nullptr;
			unit.used = 0;
			unit.StreamUnitID = 0;
			// unit.dword4 = 0;
			// unit.FrameCount_B4 = 0;
			// unit.flagsA = 0;
			// unit.hidden9 = 0;
			unit.resolveObject = nullptr;
			// unit.word9A = 0;
			// unit.dword9C = 0;
			// unit.coreUnit = nullptr;
		}
	}
}

StreamUnit *STREAM_GetStreamUnitWithID(int32_t id) { // line 1170
	for (auto &unit : StreamTracker)
		if ((unit.used == 1 || unit.used == 2) && unit.StreamUnitID == id)
			return &unit;
	return nullptr;
}

StreamUnit *STREAM_GetStreamUnitWithName(const char *name) { // line 1256
	for (auto &unit : StreamTracker)
		if (unit.used != 0 && unit.used != 4 && strcmp((char const*)unit.name, name) == 0) // HACK: stricmp, really
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

void STREAM_FinishLoad(StreamUnit *unit) { // line 1658
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

void STREAM_LoadLevelReturn(void *loadData, void *data, void *data2, ResolveObject *resolveObj) { // line 1922
	StreamUnit *unit = (StreamUnit*)data2;
	unit->StreamUnitID = ((Level*)loadData)->streamUnitID;
	// TODO
	STREAM_FinishLoad(unit);
	// TODO
}

void STREAM_StreamLoadLevelReturn(void *loadData, void *data, void *data2, ResolveObject *resolveObj) { // line 1971
	StreamUnit *unit = (StreamUnit*)data2;
	unit->StreamUnitID = ((Level*)loadData)->streamUnitID;
	// TODO
	STREAM_FinishLoad(unit);
	// TODO
}

StreamUnit *STREAM_LoadLevel(const char *baseAreaName, StreamUnitPortal *streamPortal, bool loadObjects) { // line 2085
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
void STREAM_LoadLevel(const char *baseAreaName) { // line 2171
	// TODO
}

void STREAM_RemoveInstancesWithIDInInstanceList(StreamUnit *streamUnit, bool doSave) { // line 2398
	// TODO
	for (auto instance = InstanceManager::s_chain; instance;) {
		auto n = instance->next;
		if (instance->m_streamComponent__m_birthStreamUnitID == streamUnit->StreamUnitID) {
			if (doSave) {
				// TODO
			}
			instance->ReallyRemoveInstance(); // HACK
		}
		// TODO
		instance = n;
	}
}

void STREAM_DumpUnitScript(StreamUnit *unit) { // line 2511
	// TODO
}

void STREAM_DumpUnit(StreamUnit *unit, bool doSave) { // line 2524
	printf("STREAM_DumpUnit %p StreamUnitID=%d\n", unit, unit->StreamUnitID);
	// TODO
	STREAM_RemoveInstancesWithIDInInstanceList(unit, doSave);
	// TODO
	uint32_t previousUseState = unit->used;
	unit->used = 4;
	// TODO
	STREAM_DumpUnitScript(unit);
	// TODO
	if (false) {
		// TODO
	} else {
		// IntroManager::RemoveIntrosInUnit()
		// TODO
		SceneLayer::StreamOut(unit);
		//  ObjectStreamingCallback::UnitDumped
		//      releaseObject
	}
}

void STREAM_DumpAllLevels(int32_t IDNoRemove, bool DoSave) { // line 2741

	for (auto &unit : StreamTracker)
		if (unit.used != 0 && unit.used != 4 && unit.StreamUnitID != IDNoRemove && unit.coreUnit)
			STREAM_DumpUnit(&unit, DoSave);

	for (auto &unit : StreamTracker)
		if (unit.used != 0 && unit.used != 4 && unit.StreamUnitID != IDNoRemove)
			STREAM_DumpUnit(&unit, DoSave);

	// gameTracker.unit0 = nullptr;
	// FX_GarbageCollect();
	// GAME_GetNotifier()->DumpLevel();
}

StreamUnit *STREAM_LevelLoadAndInit(const char *baseAreaName) { // line 2855
	// TODO

	if (StreamUnit *preexisting = STREAM_GetStreamUnitWithName(baseAreaName)) {
		// HACK
		return preexisting;
	}

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

class ObjectStreamingCallback : public StreamingCallback { // line 4088
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
			intro->field3C |= 0x40;
		}
	}
	void UnitDumped(StreamUnit *unit) override {
		// called from STREAM_DumpUnit via SceneLayer::StreamOut
		printf("ObjectStreamingCallback::UnitDumped(%p)\n", unit);
		dtp::ADMD *admd = unit->level->admdData;
		for (uint32_t i = 0; i < admd->numObjects; i++) {
			dtp::Intro *intro = &admd->objects[i];
			// TODO
			if (intro->field3C & 0x40) {
				releaseObject(intro->objectListIndex);
				intro->field3C &= ~0x40;
			}
		}
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
