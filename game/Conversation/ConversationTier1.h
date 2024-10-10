#pragma once
#include "ConversationBase.h"

class ConversationTier1Data : public ConversationEventData<dtp::ConversationTier1> {
public:
	// empty
};

template <typename T=dtp::ConversationGraph>
class ConversationTier1 : public ConversationBase {
public:
	ConversationTier1(
		T *dtpGraph,
		ConversationManager *manager,
		void *interactionSubData)
	:
		ConversationBase(manager, dtpGraph, interactionSubData)
	{
		// TODO
	}

	// void method_0() override;
	// void method_4() override;
	// void method_8() override;
	// void method_C() override;
	// void method_10() override;
	// void method_14() override;
	// void method_18() override;
	// void method_1C() override;
	// void method_20() override;
	// void method_24() override;
	// void method_28() override;
	// void method_2C() override;
	// void method_30() override;
	// void method_34() override;
	// void method_38() override;
	// void method_3C() override;
	// void method_40() override;
	// void method_44() override;
	// void method_48() override;
	// void method_4C() override;
	// void method_50() override;
	// void method_54() override;
	// void method_58() override;
	// void method_5C() override;
	// void method_60() override;
	// void method_64() override;
	// void method_68() override;
	// void method_6C() override;
	// void method_70() override;
	// void method_74() override;
	// void method_78() override;
	// void method_7C() override;
	// void method_80() override;
	// void method_84() override;
};
