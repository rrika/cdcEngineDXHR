#pragma once
#include "ConversationBase.h"
#include "ConversationGraph.h"

class ConversationBriefingData : public ConversationEventData<dtp::ConversationBriefing> {
public:
	ConversationBriefingData(dtp::ConversationBriefing *dtp) : ConversationEventData(dtp) {
		// TODO
	}
	dtp::ConversationBriefing *dtpCopy; // C
};

template <typename T=dtp::ConversationGraph_Briefing>
class ConversationBriefing : public ConversationBase {
	ConversationBriefingData *m_eventData = nullptr; // 88
	ConversationGraph<T> m_graph; // 8C
	uint32_t m_dwordB4 = 0;
	uint8_t m_byteB8 = 0;
	uint8_t m_byteB9;
	void *m_brieferMovieController = nullptr; // BC
	uint32_t m_instanceId; // C0
	uint32_t m_sectionId; // C4
	uint8_t m_byteC8 = 0;
	uint32_t m_dwordCC = ~0u;

public:
	ConversationBriefing(T *dtpGraph, ConversationManager*, cdc::CdcPlayer*,
		uint32_t instanceId, uint32_t uberObjectSectionId);

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
	void start() override;
	// void method_80() override;
	// void loadObject() override;
};
