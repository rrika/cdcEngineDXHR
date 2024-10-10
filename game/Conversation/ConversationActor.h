#pragma once
#include <cstdint>

class Instance;

class ConversationActor {
	uint8_t m_byte6;
public:
	ConversationActor(ConversationBase*, Instance*, uint32_t, uint32_t) {
		// TODO
	}
	virtual ~ConversationActor() = default;
	// virtual void method_04();
	// virtual void method_08();
	// virtual void method_0C();
	// virtual void method_10() {}
	// virtual void method_14();
	// virtual bool method_18() { return false; }
	// virtual uint8_t method_1C() { return m_byte6; }
	virtual void method_20() = 0;
	virtual void method_24() = 0;
	// virtual void method_28();
	// virtual void method_2C();
	// virtual void method_30() {}
};
