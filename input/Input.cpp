#include "Input.h"

namespace cdc {

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

}
