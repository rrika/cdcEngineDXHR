#pragma once
#include "cdc/dtp/postprocessing.h"

struct GlobalLoading {
	char unknown[0x144];
	dtp::PPDefaultInputs *pp_defaultInputs; // 144
	dtp::PPActiveSet *pp_cinefx;     // 148
	uint32_t dword14C;
	uint32_t dword150;
	dtp::PPActiveSet *pp_animation;  // 154
	uint32_t inputIndex158; // used in CameraOperation_AnimationDOF
	uint32_t inputIndex15C; // used in CameraOperation_AnimationDOF
	uint32_t dword160;
	uint32_t dword164;
	uint32_t dword168;
	uint32_t dword16C;
	dtp::PPActiveSet *pp_scaleform;  // 170
	dtp::PPActiveSet *pp_health;     // 174
	uint32_t dword178;
	dtp::PPActiveSet *pp_concussion; // 17C
	uint32_t dword180;
	dtp::PPActiveSet *pp_gas;        // 184
	uint32_t dword188;
	dtp::PPActiveSet *pp_alcohol;    // 18C
	uint32_t dword190;
	dtp::PPActiveSet *pp_emp;        // 194
	uint32_t dword198;
	dtp::PPActiveSet *pp_gamemenu;   // 19C
	dtp::PPActiveSet *pp_xray;       // 1A0
	uint32_t dword1A4;
	uint32_t dword1A8;
	uint32_t dword1AC;
	uint32_t dword1B0;
	uint32_t dword1B4;
	uint32_t dword1B8;
	uint32_t dword1BC;
	dtp::PPActiveSet *pp_1C0;        // 1C0
};
