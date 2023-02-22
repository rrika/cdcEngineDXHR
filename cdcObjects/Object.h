#pragma once
#include "cdcWorld/cdcWorldTypes.h"
#include "cdc/dtp/objectbasedata.h"

namespace cdc {

struct Object {
	dtp::ObjectBaseData *dtpData;
	uint16_t *punsigned4;
	uint32_t dword8;
	uint32_t dwordC;
	uint32_t dword10;
	uint32_t dword14; // own object list id
	uint32_t dword18;
	void **dword1C;
	uint16_t word20;
	uint8_t f22[10];
	uint32_t dword2C;
	uint32_t dword30;
	uint16_t word34;
	uint16_t numModels;
	uint16_t uberFake1;
	uint16_t uberFake2;
	uint16_t word3C;
	uint8_t f3E[2];
	dtp::Model **models;
	uint32_t dword44;
	uint32_t dword48;
	uint32_t dword4C;
	uint32_t dword50;
	dtp::ObjectBaseData *dtpDataCopy; // 54
	void *data; // 58
	uint32_t name;
	uint32_t dword60;
	uint16_t word64;
	uint16_t word66;
	uint16_t word68;
	uint16_t word6A;
	uint32_t dword6C_lod1_distance_maybe;
	uint32_t dword70_lod2_distance_maybe;
	uint8_t byte74_lod1_model_selector_maybe;
	uint8_t byte75_lod2_model_selector_maybe;
	uint16_t word76;
	uint32_t dword78;
	uint32_t dword7C;
	uint32_t dword80;
	uint32_t dword84;
	uint32_t dword88;
};

}
