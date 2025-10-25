#include "Input.h"
#include <cmath>

namespace cdc {

InputSystem *g_inputSystems[4] = {0};

InputState& InputState::operator+=(InputState const& other) {
	for (uint32_t i=0; i<85; i++) {
		int x = keys[i] + other.keys[i];
		if (x < -127) x = -127;
		if (x >  127) x =  127;
		keys[i] = x;
	}
	deltaX += other.deltaX;
	deltaY += other.deltaY;
	deltaWheel += other.deltaWheel;
	return *this;
}

void InputProducer::method_0() { /*TODO*/ }
void InputProducer::setCursorGrab(bool active) { cursorGrab = active; }
void InputProducer::method_8() { /*TODO*/ }
void InputProducer::method_C() { /*TODO*/ }
void InputProducer::method_10() { /*TODO*/ }
void InputProducer::update() { /*TODO*/ }
void InputProducer::method_18() { /*TODO*/ }
void InputProducer::method_1C() { /*TODO*/ }
void InputProducer::method_20() { /*TODO*/ }
void InputProducer::method_24() { /*TODO*/ }
void InputProducer::method_28() { /*TODO*/ }
void InputProducer::method_2C() { /*TODO*/ }
void InputProducer::method_30() { /*TODO*/ }
void InputProducer::method_34() { /*TODO*/ }
void InputProducer::method_38() { /*TODO*/ }
void InputProducer::method_3C() { /*TODO*/ }


InputSystem::InputSystem(uint32_t index) : index(index) {
	// TODO
}

void InputSystem::AddProducer(InputProducer *producer) {
	if (!producer)
		return;

	RemoveProducer(producer);
	producers.push_back(producer);

	// TODO: olapi
}

void InputSystem::RemoveProducer(InputProducer *producer) {
	auto it = producers.begin();
	for (auto it = producers.begin(); it != producers.end(); ++it)
		if (producer == *it)
			break;
	if (it != producers.end())
		producers.erase(it);
}

bool InputSystem::CombineInputs(uint32_t inputFrame) {
	InputState state = {0};
	for (auto *producer : producers)
		state += producer->state;

	// HACK
	inputStates[1] = inputStates[0];
	inputStates[0].state = state;
	previousState = &inputStates[1].state;
	currentState = &inputStates[0].state;
	return true;
}

bool InputSystem::IsKeyDown(EInput i) {
	return currentState->keys[i];
}

bool InputSystem::IsKeyUp(EInput i) {
	return !currentState->keys[i];
}

bool InputSystem::IsKeyPressed(EInput i) {
	return currentState->keys[i] && !previousState->keys[i];
}

bool InputSystem::IsKeyReleased(EInput i) {
	return !currentState->keys[i] && previousState->keys[i];
}

uint32_t InputSystem::GetDUPR(EInput i) {
	uint32_t v = 0;
	if (IsKeyDown(i))
		v |= 2;
	if (IsKeyUp(i))
		v |= 4;
	if (IsKeyPressed(i))
		v |= 8;
	if (IsKeyReleased(i))
		v |= 16;
	return v;
}

float InputSystem::GetValue(EInput i) {
	switch (i) {
	default: return currentState->keys[i] / 127.0;
	case /*82*/ Input_MouseX:     return currentState->deltaX;
	case /*83*/ Input_MouseY:     return currentState->deltaY;
	case /*84*/ Input_MouseWheel: return currentState->deltaWheel;
	}
}

float InputSystem::GetNormalizedValue(EInput i, Deadzone *deadzone) {
	if (i == /*32*/ Input_MovementAD ||
	    i == /*33*/ Input_MovementWS ||
	    i == /*82*/ Input_MouseX ||
	    i == /*83*/ Input_MouseY)
		return GetValue(i);

	EInput j = (EInput)85 /*invalid*/;
	switch (i) {
	case Input_16: j = Input_17; break;
	case Input_17: j = Input_16; break;
	case Input_18: j = Input_19; break;
	case Input_19: j = Input_18; break;
	default: break;
	}

	float values[2] = { GetValue(i), GetValue(j) };
	return normalize(values, deadzone, /*clamp=*/ true);
}

float normalize(float input[2], Deadzone *deadzone, bool clamp) {
	float x = input[0];
	float y = input[1];
	float dist = sqrtf(x*x+y*y);
	if (dist > 1.0) { // constrain to unit disk
		dist = 1.0;
		x /= dist;
		y /= dist;
	}

	// TODO

	if (clamp) {
		if (x >= 1.0)
			return 1.0;
		if (x <= -1.0)
			return -1.0;
	}
	return x;
}

}
