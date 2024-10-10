#include "ConversationManager.h"
#include "ConversationTier1.h"
#include "ConversationTier2.h"
#include "cdcWorld/Player.h"
#include "game/dtp/conversation.h"

void *ConversationManager::create(cdc::CdcPlayer *player, NsInstance *instance, dtp::Interaction *interaction) {
	if (!instance || !interaction)
		return nullptr;
	// TODO
	uint32_t tier = interaction->dtpGraph->tier;
	ConversationBase *c;
	if (tier == 1)
		c = new ConversationTier1<>(
			static_cast<dtp::ConversationGraph*>(interaction->dtpGraph),
			this,
			(void*)(&interaction->conversationData8));
	else if (tier == 2)
		c = new ConversationTier2<>(
			static_cast<dtp::ConversationGraphTier2*>(interaction->dtpGraph),
			this,
			(void*)(&interaction->conversationData8));
	else
		return nullptr;

	return c;
}
