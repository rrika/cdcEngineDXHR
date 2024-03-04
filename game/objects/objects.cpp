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

	om->RegisterObjectFamily(3, // 0x03
		nullptr, // TODO: Init
		nullptr, // TODO: Uninit
		nullptr, // TODO: Process
		nullptr, // TODO: Collide
		nullptr, // TODO: AdditionalCollide
		nullptr, // TODO: Query,
		nullptr, // TODO: Message,
		nullptr, // TODO: DebugDraw,
		nullptr);
	om->RegisterObjectFamily(83, // 0x53
		nullptr, // TODO: Init
		nullptr, // TODO: Uninit
		nullptr, // TODO: Process
		nullptr, // TODO: Collide
		nullptr, // TODO: AdditionalCollide
		nullptr, // TODO: Query,
		nullptr, // TODO: Message,
		nullptr, // TODO: DebugDraw,
		nullptr);
	om->RegisterObjectFamily(85, // 0x55
		nullptr, // TODO: Init
		nullptr, // TODO: Uninit
		nullptr, // TODO: Process
		nullptr, // TODO: Collide
		nullptr, // TODO: AdditionalCollide
		nullptr, // TODO: Query,
		nullptr, // TODO: Message,
		nullptr, // TODO: DebugDraw,
		nullptr);
	om->RegisterObjectFamily(2, // 0x02
		nullptr, // TODO: Init
		nullptr, // TODO: Uninit
		nullptr, // TODO: Process
		nullptr, // TODO: Collide
		nullptr, // TODO: AdditionalCollide
		nullptr, // TODO: Query,
		nullptr, // TODO: Message,
		nullptr, // TODO: DebugDraw,
		nullptr);
	om->RegisterObjectFamily(50, // 0x32
		nullptr, // TODO: Init
		nullptr, // TODO: Uninit
		nullptr, // TODO: Process
		nullptr, // TODO: Collide
		nullptr, // TODO: AdditionalCollide
		nullptr, // TODO: Query,
		nullptr, // TODO: Message,
		nullptr, // TODO: DebugDraw,
		nullptr);
	om->RegisterObjectFamily(61, // 0x3D
		nullptr, // TODO: Init
		nullptr, // TODO: Uninit
		nullptr, // TODO: Process
		nullptr, // TODO: Collide
		nullptr, // TODO: AdditionalCollide
		nullptr, // TODO: Query,
		nullptr, // TODO: Message,
		nullptr, // TODO: DebugDraw,
		nullptr);
	om->RegisterObjectFamily(90, // 0x5A
		nullptr, // TODO: Init
		nullptr, // TODO: Uninit
		nullptr, // TODO: Process
		nullptr, // TODO: Collide
		nullptr, // TODO: AdditionalCollide
		nullptr, // TODO: Query,
		nullptr, // TODO: Message,
		nullptr, // TODO: DebugDraw,
		nullptr);
	om->RegisterObjectFamily(7, // 0x07
		nullptr, // TODO: Init
		nullptr, // TODO: Uninit
		nullptr, // TODO: Process
		nullptr, // TODO: Collide
		nullptr, // TODO: AdditionalCollide
		nullptr, // TODO: Query,
		nullptr, // TODO: Message,
		nullptr, // TODO: DebugDraw,
		nullptr);
	om->RegisterObjectFamily(88, // 0x58
		nullptr, // TODO: Init
		nullptr, // TODO: Uninit
		nullptr, // TODO: Process
		nullptr, // TODO: Collide
		nullptr, // TODO: AdditionalCollide
		nullptr, // TODO: Query,
		nullptr, // TODO: Message,
		nullptr, // TODO: DebugDraw,
		nullptr);
	om->RegisterObjectFamily(0, // 0x00
		nullptr, // TODO: Init
		nullptr, // TODO: Uninit
		nullptr, // TODO: Process
		nullptr, // TODO: Collide
		nullptr, // TODO: AdditionalCollide
		nullptr, // TODO: Query,
		nullptr, // TODO: Message,
		nullptr, // TODO: DebugDraw,
		nullptr);
	om->RegisterObjectFamily(81, // 0x51
		nullptr, // TODO: Init
		nullptr, // TODO: Uninit
		nullptr, // TODO: Process
		nullptr, // TODO: Collide
		nullptr, // TODO: AdditionalCollide
		nullptr, // TODO: Query,
		nullptr, // TODO: Message,
		nullptr, // TODO: DebugDraw,
		nullptr);
	om->RegisterObjectFamily(52, // 0x34
		nullptr, // TODO: Init
		nullptr, // TODO: Uninit
		nullptr, // TODO: Process
		nullptr, // TODO: Collide
		nullptr, // TODO: AdditionalCollide
		nullptr, // TODO: Query,
		nullptr, // TODO: Message,
		nullptr, // TODO: DebugDraw,
		nullptr);
	om->RegisterObjectFamily(30, // 0x1E
		nullptr, // TODO: Init
		nullptr, // TODO: Uninit
		nullptr, // TODO: Process
		nullptr, // TODO: Collide
		nullptr, // TODO: AdditionalCollide
		nullptr, // TODO: Query,
		nullptr, // TODO: Message,
		nullptr, // TODO: DebugDraw,
		nullptr);
	om->RegisterObjectFamily(33, // 0x21
		nullptr, // TODO: Init
		nullptr, // TODO: Uninit
		nullptr, // TODO: Process
		nullptr, // TODO: Collide
		nullptr, // TODO: AdditionalCollide
		nullptr, // TODO: Query,
		nullptr, // TODO: Message,
		nullptr, // TODO: DebugDraw,
		nullptr);
	om->RegisterObjectFamily(32, // 0x20
		nullptr, // TODO: Init
		nullptr, // TODO: Uninit
		nullptr, // TODO: Process
		nullptr, // TODO: Collide
		nullptr, // TODO: AdditionalCollide
		nullptr, // TODO: Query,
		nullptr, // TODO: Message,
		nullptr, // TODO: DebugDraw,
		nullptr);
	om->RegisterObjectFamily(34, // 0x22
		nullptr, // TODO: Init
		nullptr, // TODO: Uninit
		nullptr, // TODO: Process
		nullptr, // TODO: Collide
		nullptr, // TODO: AdditionalCollide
		nullptr, // TODO: Query,
		nullptr, // TODO: Message,
		nullptr, // TODO: DebugDraw,
		nullptr);
	om->RegisterObjectFamily(24, // 0x18
		nullptr, // TODO: Init
		nullptr, // TODO: Uninit
		nullptr, // TODO: Process
		nullptr, // TODO: Collide
		nullptr, // TODO: AdditionalCollide
		nullptr, // TODO: Query,
		nullptr, // TODO: Message,
		nullptr, // TODO: DebugDraw,
		nullptr);
	om->RegisterObjectFamily(10, // 0x0A
		nullptr, // TODO: Init
		nullptr, // TODO: Uninit
		nullptr, // TODO: Process
		nullptr, // TODO: Collide
		nullptr, // TODO: AdditionalCollide
		nullptr, // TODO: Query,
		nullptr, // TODO: Message,
		nullptr, // TODO: DebugDraw,
		nullptr);
	om->RegisterObjectFamily(89, // 0x59
		nullptr, // TODO: Init
		nullptr, // TODO: Uninit
		nullptr, // TODO: Process
		nullptr, // TODO: Collide
		nullptr, // TODO: AdditionalCollide
		nullptr, // TODO: Query,
		nullptr, // TODO: Message,
		nullptr, // TODO: DebugDraw,
		nullptr);
	om->RegisterObjectFamily(80, // 0x50
		nullptr, // TODO: Init
		nullptr, // TODO: Uninit
		nullptr, // TODO: Process
		nullptr, // TODO: Collide
		nullptr, // TODO: AdditionalCollide
		nullptr, // TODO: Query,
		nullptr, // TODO: Message,
		nullptr, // TODO: DebugDraw,
		nullptr);
	om->RegisterObjectFamily(87, // 0x57
		nullptr, // TODO: Init
		nullptr, // TODO: Uninit
		nullptr, // TODO: Process
		nullptr, // TODO: Collide
		nullptr, // TODO: AdditionalCollide
		nullptr, // TODO: Query,
		nullptr, // TODO: Message,
		nullptr, // TODO: DebugDraw,
		nullptr);
	om->RegisterObjectFamily(91, // 0x5B
		nullptr, // TODO: Init
		nullptr, // TODO: Uninit
		nullptr, // TODO: Process
		nullptr, // TODO: Collide
		nullptr, // TODO: AdditionalCollide
		nullptr, // TODO: Query,
		nullptr, // TODO: Message,
		nullptr, // TODO: DebugDraw,
		nullptr);
	om->RegisterObjectFamily(62, // 0x3E
		nullptr, // TODO: Init
		nullptr, // TODO: Uninit
		nullptr, // TODO: Process
		nullptr, // TODO: Collide
		nullptr, // TODO: AdditionalCollide
		nullptr, // TODO: Query,
		nullptr, // TODO: Message,
		nullptr, // TODO: DebugDraw,
		nullptr);
	om->RegisterObjectFamily(82, // 0x52
		nullptr, // TODO: Init
		nullptr, // TODO: Uninit
		nullptr, // TODO: Process
		nullptr, // TODO: Collide
		nullptr, // TODO: AdditionalCollide
		nullptr, // TODO: Query,
		nullptr, // TODO: Message,
		nullptr, // TODO: DebugDraw,
		nullptr);
	om->RegisterObjectFamily(84, // 0x54
		nullptr, // TODO: Init
		nullptr, // TODO: Uninit
		nullptr, // TODO: Process
		nullptr, // TODO: Collide
		nullptr, // TODO: AdditionalCollide
		nullptr, // TODO: Query,
		nullptr, // TODO: Message,
		nullptr, // TODO: DebugDraw,
		nullptr);
	om->RegisterObjectFamily(40, // 0x28
		nullptr, // TODO: Init
		nullptr, // TODO: Uninit
		nullptr, // TODO: Process
		nullptr, // TODO: Collide
		nullptr, // TODO: AdditionalCollide
		nullptr, // TODO: Query,
		nullptr, // TODO: Message,
		nullptr, // TODO: DebugDraw,
		nullptr);
	om->RegisterObjectFamily(92, // 0x5C
		nullptr, // TODO: Init
		nullptr, // TODO: Uninit
		nullptr, // TODO: Process
		nullptr, // TODO: Collide
		nullptr, // TODO: AdditionalCollide
		nullptr, // TODO: Query,
		nullptr, // TODO: Message,
		nullptr, // TODO: DebugDraw,
		nullptr);
	om->RegisterObjectFamily(94, // 0x5E
		nullptr, // TODO: Init
		nullptr, // TODO: Uninit
		nullptr, // TODO: Process
		nullptr, // TODO: Collide
		nullptr, // TODO: AdditionalCollide
		nullptr, // TODO: Query,
		nullptr, // TODO: Message,
		nullptr, // TODO: DebugDraw,
		nullptr);
	om->RegisterObjectFamily(95, // 0x5F
		nullptr, // TODO: Init
		nullptr, // TODO: Uninit
		nullptr, // TODO: Process
		nullptr, // TODO: Collide
		nullptr, // TODO: AdditionalCollide
		nullptr, // TODO: Query,
		nullptr, // TODO: Message,
		nullptr, // TODO: DebugDraw,
		nullptr);
}
