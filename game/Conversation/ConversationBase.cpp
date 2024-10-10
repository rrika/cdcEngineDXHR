#include "ConversationBase.h"
#include "ConversationActorPlayer.h"
#include "cdcObjects/ObjectManager.h"

ConversationBase::ConversationBase(
	ConversationManager *manager,
	dtp::ConversationGraphBase *dtpGraph,
	void *interactionSubData)
{
	m_dtpGraph = dtpGraph;
	m_dtpGraphSub = nullptr; // assigned by subclass
	m_interactionSubData = interactionSubData;
	m_manager = manager;
	// TODO
}

void ConversationBase::createActorPlayer(cdc::CdcPlayer *player, uint32_t arg) {
	// TODO
	new ConversationActorPlayer(player, this, arg);
}

void ConversationBase::start() {
	// TODO
}

void ConversationBase::loadObject() {
	uint16_t objectListIndex = m_dtpGraphSub->objectListIndex;
	cdc::requestObjectHigh(objectListIndex);
	m_objectTracker = cdc::getByObjectListIndex(objectListIndex);
}
