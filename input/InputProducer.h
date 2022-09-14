#pragma once
#include <cstdint>

namespace cdc {

struct InputState {
	uint8_t keys[85];
	float deltaX; // 58
	float deltaY; // 5C
	float float60;
};

struct InputProducerSub70 { // probably just std::string
	uint32_t dword0;
	uint32_t dword4;
	uint32_t dword8;
};

struct InputProducerSub6C {
	uint32_t dword0;
	InputProducerSub70 sub4;
};

class InputProducer {
// protected:
public:
	InputState state;
	// uint8_t gap68[4];
	// InputProducerSub6C sub6C;
	// float float7C[2];
	// uint8_t byte84;
	// uint8_t byte85;
	bool cursorGrab; // 86
	// uint32_t dword88;
public:
	virtual void method_0();
	virtual void setCursorGrab(bool active);
	virtual void method_8();
	virtual void method_C();
	virtual void method_10();
	virtual void update();
	virtual void method_18();
	virtual void method_1C();
	virtual void method_20();
	virtual void method_24();
	virtual void method_28();
	virtual void method_2C();
	virtual void method_30();
	virtual void method_34();
	virtual void method_38();
	virtual void method_3C();
};

}
