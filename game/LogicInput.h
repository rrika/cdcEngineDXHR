#include <cstdint>

struct LogicInputNode;
struct LogicInputBlob;
struct LogicInputHandlerY;

struct LogicInput {
	uint8_t m_returnOneOnce;
	uint8_t m_useFixedValue;
	uint8_t m_fixedValue;
	LogicInputBlob *m_dtpData; // 4
	uint32_t m_dtpIndex; // 8
	LogicInputHandlerY *handlers[4] = {0}; // C
	uint32_t dword1C = 0;
	float m_time[4]; // 20
	uint8_t m_byte30[4];
	uint16_t m_sequenceChildIndex[4]; // 34
	uint8_t m_cachedSequenceActivation[4]; // 3C

	void fromDTP(uint32_t);
	void resetSequences(uint32_t);
	void resetSequences(LogicInputNode *node, uint32_t);
	int32_t evalNodeInt(LogicInputNode *node, uint32_t inputSystemIndex);
	float evalNodeFloat(LogicInputNode *node, bool queryFloatDirectly, uint32_t inputSystemIndex);
	float evalFloatInternal(uint32_t inputSystemIndex);

	int32_t evalInt(uint32_t inputSystemIndex, bool);
	float evalFloat(uint32_t inputSystemIndex, bool);
	void forceActive(uint32_t unused);
	uint32_t getDUPR(uint32_t inputSystemIndex);
	void timeThresholds(LogicInputNode *node, uint32_t inputSystemIndex);
};

struct LogicInputNode0or5;
struct LogicInputNode1;
struct LogicInputNode2;
struct LogicInputNode3;
struct LogicInputNode4;

struct LogicInputNode {
	uint32_t type;
	void *dword4;
	uint8_t prevInRange;

	uint32_t getDUPR(uint32_t inputSystemIndex);
	static uint32_t getKeyIndex(LogicInputNode *node);

	LogicInputNode0or5 *asButtonNode() {
		return (type==0||type==5) ? (LogicInputNode0or5*) dword4 : nullptr;
	}
	LogicInputNode1 *asAxisNode() {
		return type==1 ? (LogicInputNode1*) dword4 : nullptr;
	}
	LogicInputNode2 *asGateNode() {
		return type==2 ? (LogicInputNode2*) dword4 : nullptr;
	}
	LogicInputNode3 *asSequenceNode() {
		return type==3 ? (LogicInputNode3*) dword4 : nullptr;
	}
	LogicInputNode4 *asIndirectNode() {
		return type==4 ? (LogicInputNode4*) dword4 : nullptr;
	}
};

struct LogicInputBlob {
	const char *name;
	LogicInputNode mainNode;
};

struct LogicInputNode0or5Key {
	uint32_t keyindex;
};

struct LogicInputNode0or5 {
	uint32_t dword0;
	LogicInputNode0or5Key key0; // 4
	uint32_t mode; // 8
	float analogueValue; // C
	LogicInputNode0or5Key key5; // 10
};

struct LogicInputNode2 {
	uint32_t any; // 0, 0=all, 1=any
	uint32_t count; // 4
	LogicInputNode *nodes; // 8
};

struct LogicInputNode1 {
	uint32_t dword0; // 0
	uint32_t axis; // 4
	float min; // 8
	float max; // C
	uint32_t mode; // 10
	uint8_t hasCustomDeadzone; // 14
	// Deadzone follows at offset 18
};

struct LogicInputNodeExt {
	float timeThreshold;
	uint8_t byte4;
	uint8_t byte5;
		// 0 = activation advances, timeout resets
		// 1 = deactivation resets, timeout advances
	LogicInputNode node;
};

struct LogicInputNode3 {
	uint32_t dword0;
	uint8_t byte4;
	uint8_t byte5;
	uint8_t byte6;
	uint8_t activateAtStart; // 7
	uint8_t activateAtCompletion; // 8
	uint8_t byte9;
	uint8_t byteA;
	uint8_t byteB;
	LogicInputNode node; // C
	uint32_t childCount; // 18
	LogicInputNodeExt *nodesExt; // 1C
};

struct LogicInputNode4 {
	uint32_t dtpIndex;
};

struct LogicInputHandler {
	virtual void method() = 0;
};

struct LogicInputHandlerX {
	uint32_t dword0;
	uint32_t dword4;
	LogicInputHandler *handler8;
};

struct LogicInputHandlerY {
	LogicInputHandlerX *dword0;
};

struct UIActions;
void logicinput_load_database();
void logicinput_build_ui(UIActions& uiact);
