#pragma once
#include <cstdint>
#include "../../../../scene/cdcSceneCookdata.h"

namespace dtp {

struct Intro {
	float rotation[4];
	float position[4];
	float scale[4];
	uint16_t objectListIndex;
	uint16_t word32;
	uint8_t padding1[4];
	uint32_t field38;
	uint32_t field3C;
	uint8_t field40;
	uint8_t field41;
	uint8_t flags;
	uint8_t field43;
	uint32_t field44_extraData1;
	uint32_t field48;
	char field_4C[12];
	uint32_t field58_extraData2;
	cdc::CellStreamGroupData *sceneCellIdent;
	char field_60[16];
};

}