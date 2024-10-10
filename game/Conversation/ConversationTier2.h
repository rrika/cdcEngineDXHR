#pragma once
#include <cstdint>
#include "ConversationBase.h"

class ConversationData : public ConversationEventData<dtp::ConversationTier2v2> {
public:
	// uint16_t wordC;
	// uint16_t wordE;
	// uint8_t byte10;
	// uint8_t byte11;
	// uint8_t byte12;
	// uint32_t dword14;
	// uint16_t word18;
	// float float1C;
	// float float20;
	// uint16_t word24;
	// float float28;
	// float float2C;
	// float float30;
	// uint8_t gap34[8];
	// uint8_t byte3C;
	// uint32_t dword40;
	// uint32_t dword44;
};

template <typename T=dtp::ConversationGraphTier2>
class ConversationTier2 : public ConversationBase {
public:
	ConversationTier2(
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
