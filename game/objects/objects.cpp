#include "objects.h"
#include "../../object/ObjectManager.h"

struct GameTracker;

void GameAdditionalPreInit(Instance*, GameTracker*) {
	// TODO
}

void GameAdditionalPostInit(Instance*, GameTracker*) {
	// TODO
}

void GameAdditionalUninit(Instance*, GameTracker*) {
	// TODO
}

void RegisterGameObjects() {
	auto *om = cdc::g_objectManager;
	om->preInit = &GameAdditionalPreInit;
	om->postInit = &GameAdditionalPostInit;
	om->uninit = &GameAdditionalUninit;

	// TODO
}
