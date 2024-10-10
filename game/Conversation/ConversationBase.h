#pragma once
#include <cstdint>
#include "cdcSys/RCObject.h"
#include "game/dtp/conversation.h"

namespace cdc {

class CdcPlayer;
struct ObjectTracker;

}

class ConversationManager;

template <typename T>
class ConversationEventData {
public:
	ConversationEventData(T *dtp) : dtp(dtp) {
		// TODO
	}
	virtual ~ConversationEventData() = default;
	virtual T *getDtp() { return dtp; }
	// RCPtr<> ptr4;
	T *dtp; // 8
};

class ConversationBase : public cdc::RCObject {
	// see ConversationBriefing
	// see ConversationTier1
	// see ConversationTier2

protected:
	// uint32_t dword8;
	// uint32_t stateC;
	dtp::ConversationGraphBase *m_dtpGraph; // 10
	dtp::ConversationBase *m_dtpGraphSub; // 14
	void *m_interactionSubData; // 18
	ConversationManager *m_manager; // 1C
	// uint8_t byte20;
	// uint8_t byte21;
	// uint8_t byte22;
	// uint32_t nsQuest24;
	// uint8_t byte28;
	// float float2C;
	// float float30;
	// hashmap hashmap_34;
	// hashmap hashmap_48;
	// uint32_t dword5C;
	cdc::ObjectTracker *m_objectTracker;
	// uint8_t byte64;
	// uint8_t byte65;
	// uint8_t byte66;
	// uint32_t dword68;
	// uint32_t dword6C;
	// uint32_t dword70;
	// uint32_t dword74;
	// uint8_t byte78;
	// float float7C;
	// uint8_t byte80;
	// PersonalityTypeMovie *personalityMovie84;

public:
	ConversationBase(ConversationManager*, dtp::ConversationGraphBase *dtpGraph, void *interactionSubData);

	void createActorPlayer(cdc::CdcPlayer *player, uint32_t);

	// virtual void method_0();
	// virtual void method_4();
	// virtual void method_8();
	// virtual void method_C() = 0;
	// virtual void method_10();
	// virtual void method_14();
	// virtual void method_18() = 0;
	// virtual void method_1C();
	// virtual void method_20();
	// virtual void method_24();
	// virtual void method_28();
	// virtual void method_2C() = 0;
	// virtual void method_30();
	// virtual void method_34();
	// virtual void method_38() = 0;
	// virtual void method_3C();
	// virtual void method_40();
	// virtual void method_44();
	// virtual void method_48() = 0;
	// virtual void method_4C();
	// virtual void method_50();
	// virtual void method_54() = 0;
	// virtual void method_58();
	// virtual void method_5C() = 0;
	// virtual void method_60() = 0;
	// virtual void method_64() = 0;
	// virtual void method_68();
	// virtual void method_6C();
	// virtual void method_70();
	// virtual void method_74();
	// virtual void method_78();
	virtual void start(); // 7C
	// virtual void method_80();
	virtual void loadObject(); // 84
};
