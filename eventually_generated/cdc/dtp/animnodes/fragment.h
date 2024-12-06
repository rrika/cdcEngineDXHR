#pragma once

namespace dtp {

struct EventEnum {
	uint32_t dword0;
	uint32_t dword4;
};

struct FragmentNode {
	uint32_t extIndex;

	uint32_t dword04;
	uint32_t m_loopMode; //8
	float m_playspeed; // C
	EventEnum *m_loopTriggers; // 10
	EventEnum *m_endTriggers; // 14
	uint32_t m_loopTriggerCount; // 18
	uint32_t m_endTriggerCount; // 1C
	uint32_t m_playFXMode; // 20
	uint32_t m_triggerTransitionMode; // 24
	uint32_t dword28;
	uint32_t dword2C;
};

}
