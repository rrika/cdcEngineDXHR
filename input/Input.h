#pragma once
#include <cstdint>
#include <vector>

namespace cdc {

enum EKeyboard {
	Keyboard_MovementLeft,
	Keyboard_MovementRight,
	Keyboard_MovementForward,
	Keyboard_MovementBackward,
	Keyboard_MovementRunToggle,
	Keyboard_MovementWalk,
	Keyboard_Action,
	Keyboard_Jump,
	Keyboard_Sprint,
	Keyboard_Crouch,
	Keyboard_Cover,
	Keyboard_WeaponFire,
	Keyboard_WeaponReload,
	Keyboard_WeaponHolster,
	Keyboard_WeaponIronSight,
	Keyboard_ThrowDetonate,
	Keyboard_Takedown,
	Keyboard_Cloak,
	Keyboard_Claymore,
	Keyboard_MoveSilently,
	Keyboard_VisionEnhancer,
	Keyboard_SpecialUpgrade,
	Keyboard_TechTree,
	Keyboard_Inventory,
	Keyboard_Objectives,
	Keyboard_Logs,
	Keyboard_Map,
	Keyboard_QuickBarShow,
	Keyboard_QuickBarNext,
	Keyboard_QuickBarPrev,
	Keyboard_MarkAndTrack,
	Keyboard_TriggerCommentary
};

enum EInput {
	Input_MovementAD = 32,
	Input_MovementWS = 33,

	Input_Action = 36,
		Input_KeyboardOffset = Input_Action - Keyboard_Action,
	Input_Jump = Input_KeyboardOffset + Keyboard_Jump,
	Input_Sprint = Input_KeyboardOffset + Keyboard_Sprint,
	Input_Crouch = Input_KeyboardOffset + Keyboard_Crouch,
	Input_Cover = Input_KeyboardOffset + Keyboard_Cover,
	Input_WeaponFire = Input_KeyboardOffset + Keyboard_WeaponFire,
	Input_WeaponReload = Input_KeyboardOffset + Keyboard_WeaponReload,
	Input_WeaponHolster = Input_KeyboardOffset + Keyboard_WeaponHolster,
	Input_WeaponIronSight = Input_KeyboardOffset + Keyboard_WeaponIronSight,
	Input_ThrowDetonate = Input_KeyboardOffset + Keyboard_ThrowDetonate,
	Input_Takedown = Input_KeyboardOffset + Keyboard_Takedown,
	Input_Cloak = Input_KeyboardOffset + Keyboard_Cloak,
	Input_Claymore = Input_KeyboardOffset + Keyboard_Claymore,
	Input_MoveSilently = Input_KeyboardOffset + Keyboard_MoveSilently,
	Input_VisionEnhancer = Input_KeyboardOffset + Keyboard_VisionEnhancer,
	Input_SpecialUpgrade = Input_KeyboardOffset + Keyboard_SpecialUpgrade,
	Input_TechTree = Input_KeyboardOffset + Keyboard_TechTree,
	Input_Inventory = Input_KeyboardOffset + Keyboard_Inventory,
	Input_Objectives = Input_KeyboardOffset + Keyboard_Objectives,
	Input_Logs = Input_KeyboardOffset + Keyboard_Logs,
	Input_Map = Input_KeyboardOffset + Keyboard_Map,
	Input_QuickBarShow = Input_KeyboardOffset + Keyboard_QuickBarShow,
	Input_QuickBarNext = Input_KeyboardOffset + Keyboard_QuickBarNext,
	Input_QuickBarPrev = Input_KeyboardOffset + Keyboard_QuickBarPrev,
	Input_MarkAndTrack = Input_KeyboardOffset + Keyboard_MarkAndTrack,
	Input_TriggerCommentary = Input_KeyboardOffset + Keyboard_TriggerCommentary,

	Input_MouseX = 82,
	Input_MouseY = 83,
	Input_MouseWheel = 84
};

struct InputState {
	char keys[85];
	float deltaX; // 58
	float deltaY; // 5C
	float deltaWheel; // 60

	InputState& operator+=(InputState const& other);
};

struct InputStateExt {
	InputState state;
	uint32_t dword64;
	uint32_t index68;
	float dword6C;
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

class InputSystem {
	std::vector<InputProducer*> producers; // 4
	InputState *currentState; // 10
	InputState *previousState; // 14
	// uint8_t byte18;
	uint32_t index; // 1C
	// uint32_t dword20;
	InputStateExt inputStates[16]; // 24
	// uint32_t dword724;
	// uint8_t byte728;
	// uint8_t byte729;

public:
	InputSystem(uint32_t i);
	virtual ~InputSystem() = default;

	void AddProducer(InputProducer*);
	void RemoveProducer(InputProducer*);
	bool CombineInputs(uint32_t inputFrame);

	bool IsKeyDown(EInput i);
	bool IsKeyUp(EInput i);
	bool IsKeyPressed(EInput i);
	bool IsKeyReleased(EInput i);
	uint32_t GetDUPR(EInput i);
	float GetValue(EInput i);
};

}
