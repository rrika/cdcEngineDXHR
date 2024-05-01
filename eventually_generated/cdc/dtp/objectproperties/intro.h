#pragma once
#include <cstdint>
#include "cdcScene/cdcSceneCookdata.h"

namespace dtp {

struct Intro {
	float rotation[4];
	float position[4];
	float scale[4];
	uint16_t objectListIndex;
	uint16_t word32;
	uint8_t padding1[4];
	uint32_t uniqueID;
	uint32_t field3C;
	uint8_t field40;
	uint8_t field41;
	uint8_t flags;
	uint8_t field43;
	void *extraData1; // 44
	uint32_t field48;
	uint32_t field4C;
	uint32_t m_scriptTypeID; // 50
	uint32_t field54;
	uint32_t field58_extraData2;
	cdc::CellStreamGroupData *sceneCellIdent;
	char field_60[16];
};

}
