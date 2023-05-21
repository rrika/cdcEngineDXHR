#include "objects.h"
#include "cdcObjects/ObjectManager.h"
#include "cdcWorld/Instance.h"

struct GameTracker;

// called from ObjectComponent::InstanceInit
void GameAdditionalPreInit(Instance*, GameTracker*) {
	// TODO
}

// called from ObjectComponent::InstanceInit
void GameAdditionalPostInit(Instance *instance, GameTracker*) {
	uint16_t familyId = instance->GetObjectComponent().m_familyId;
	switch (familyId) {
	case 0x50: /*Instance_CreateDeferredRenderingObject(instance);*/ break;
	case 0x51: /*Instance_CreatePickupInstanceDrawable(instance);*/ break;
	case 0x54: /*Instance_CreatePlanarSmokeDrawable((int)go);*/ break;
	case 0x57: /*DeferredOccluderObject::Drawable::Drawable(instance);*/ break;
	case 0x5A: /*Instance_CreateAnimatedInstanceDrawable(instance);*/ break;
	case 0x5B: /*Instance_CreateLensFlatAndCoronaDrawable(instance);*/ break;
	default: break;
	}
	// TODO
}

// called from ObjectComponent::InstanceUninit
void GameAdditionalUninit(Instance*, GameTracker*) {
	// TODO
}

void RegisterGameObjects() {
	cdc::ObjectManager *om = cdc::g_objectManager;
	om->preInit = &GameAdditionalPreInit;
	om->postInit = &GameAdditionalPostInit;
	om->uninit = &GameAdditionalUninit;

	// TODO
}
