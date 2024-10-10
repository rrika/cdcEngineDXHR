#include "ConversationBriefing.h"

template <typename T>
ConversationBriefing<T>::ConversationBriefing(
	T *dtpGraph,
	ConversationManager *manager,
	cdc::CdcPlayer *player,
	uint32_t instanceId,
	uint32_t uberObjectSectionId)
:
	ConversationBase(manager, dtpGraph, nullptr)
{
	m_instanceId = instanceId;
	m_sectionId = uberObjectSectionId;

	createActorPlayer(player, 1);

	m_eventData = new ConversationBriefingData(dtpGraph->sub8);
	m_dtpGraphSub = dtpGraph->sub8;

	// TODO: register message 3
	// TODO: register message 34
}

template <typename T>
void ConversationBriefing<T>::start() {
	ConversationBase::start();

}

template class ConversationBriefing<>; // explicit instantiation
