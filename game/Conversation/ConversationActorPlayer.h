#pragma once
#include <cstdint>
#include "ConversationActor.h"

class ConversationActorPlayer : public ConversationActor {
public:
	ConversationActorPlayer(cdc::CdcPlayer*, ConversationBase* conv, uint32_t) :
		ConversationActor(conv, nullptr, 0, 0) // TODO
	{
		// TODO
	}

	void method_20() override { /*TODO*/ }
	void method_24() override { /*TODO*/ }
};
