#pragma once
#include <cstdint>

namespace dtp {

struct PPActiveSet;

struct UnitData {
	struct depthq {
		uint8_t byte0;
		uint8_t byte1;
		uint8_t byte2;
		uint8_t byte3;
		uint8_t byte4;
		uint8_t byte5;
		uint8_t gap6[2];
		uint32_t m_numShadowMaps; // 8, probably
		uint32_t m_pssmIsAbsolute; // C, probably
		float m_pssmDistances[4]; // 10, probably
		float farPlane; // 20
		float fogFar; // 24
		float fogNear; // 28
		uint32_t dword2C;
		uint32_t dword30;
		uint32_t dword34;
		uint32_t dword38;
		uint32_t dword3C;
	};

	struct PostProcessing {
		uint8_t gap0[20];
		uint32_t ssaoMaterial_A; // 14
		uint32_t ssaoMaterial_B; // 18
		uint32_t ssaoMaterial_C; // 1C
		uint8_t ssaoMaterials_inUse; // 20
		uint8_t gap21[251];
		PPActiveSet *ppactiveset; // 11C
		uint32_t dword120;
		uint32_t dword124;
		float float128;
		uint32_t dword12C;
	};

	const char *name;
	uint32_t dword4;
	uint32_t m_numFSFX; // 8
	uint32_t m_pFSFX; // C
	depthq depthData; // 10
	uint32_t dword50;
	uint32_t dword54;
	uint32_t dword58;
	uint32_t dword5C;
	uint32_t dword60;
	uint32_t dword64;
	uint32_t dword68;
	uint32_t dword6C;
	uint32_t dword70;
	uint32_t dword74;
	uint32_t dword78;
	uint32_t dword7C;
	uint32_t dword80;
	uint32_t dword84;
	uint32_t dword88;
	uint32_t dword8C;
	uint32_t dword90;
	uint32_t dword94;
	uint32_t dword98;
	uint32_t dword9C;
	uint32_t dwordA0;
	uint32_t dwordA4;
	uint32_t dwordA8;
	uint32_t dwordAC;
	uint32_t dwordB0;
	uint32_t dwordB4;
	uint32_t dwordB8;
	uint32_t dwordBC;
	uint32_t dwordC0;
	uint32_t dwordC4;
	uint32_t dwordC8;
	uint32_t dwordCC;
	uint32_t dwordD0;
	uint32_t dwordD4;
	uint32_t dwordD8;
	uint32_t dwordDC;
	uint32_t dwordE0;
	uint32_t dwordE4;
	uint32_t dwordE8;
	uint32_t dwordEC;
	uint32_t dwordF0;
	uint32_t dwordF4;
	uint32_t dwordF8;
	uint32_t dwordFC;
	uint32_t dword100;
	uint32_t dword104;
	uint32_t dword108;
	uint32_t dword10C;
	uint32_t dword110;
	uint32_t dword114;
	uint32_t dword118;
	uint32_t dword11C;
	uint32_t dword120;
	uint32_t dword124;
	uint32_t dword128;
	uint32_t dword12C;
	uint32_t dword130;
	uint32_t dword134;
	uint32_t dword138;
	uint32_t dword13C;
	uint32_t dword140;
	PostProcessing *postprocessing; // 144
};

static_assert(sizeof(UnitData::depthq) == 0x40);

}
