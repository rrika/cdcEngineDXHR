#include "game/objects/UberObject_DX3.h"
#include "ObjectComponent.h"
#include "ObjectManager.h"
#include "cdcWorld/GameTracker.h"
#include "cdcWorld/Instance.h"
#include "cdcWorld/InstncG2.h"

namespace cdc {

// ObjectManager *g_objectManager;

// called from Instance::InitCommonComponents
void ObjectComponent::SetProcessFunctions() {
	// TODO
}

// called from Instance::InitCommonComponents
bool ObjectComponent::InstanceInit(bool unknown) {
	// TODO
	if (unknown) {
		// TODO
		m_familyId = m_instance->objectFamilyId;
	}
	g_objectManager->preInit(m_instance, &gameTrackerX); // calls GameAdditionalPreInit
	G2Instance_SetTransformsToIdentity(m_instance);
	// TODO: call the object family's init function
	//       right now I'm just hardcoding the init for UberObjects (family 2) though
	if (m_familyId == 2)
		UBEROBJECT_DX3_Init(m_instance, &gameTrackerX);
	// TODO
	g_objectManager->postInit(m_instance, &gameTrackerX);// calls GameAdditionalPostInit
	// TODO
	return true;
}

bool ObjectComponent::InstanceUninit(bool unknown) {
	// TODO
	g_objectManager->uninit(m_instance, &gameTrackerX); // calls GameAdditionalUninit
	// TODO
	return true;
}

}
