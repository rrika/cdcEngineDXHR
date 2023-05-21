#pragma once
#include <cstdint>

namespace cdc { class RenderMesh; }

namespace dtp {

struct Model { // line 516
	void *field_0;
	char gap_4[4];
	int field_8;
	void *field_C;
	void *field_10;
	void *field_14;
	void *field_18;
	int field_1C;
	int field_20;
	int field_24;
	int field_28;
	void *field_2C;
	int field_30;
	int field_34;
	int field_38;
	int field_3C;
	int field_40;
	void *field_44;
	void *field_48;
	void *field_4C;
	void *field_50;
	int field_54;
	int field_58;
	int field_5C;
	int field_60;
	cdc::RenderMesh *renderMesh;
	void *field_68;
	uint32_t *pdword6C;
	uint32_t dword70;
};

}
