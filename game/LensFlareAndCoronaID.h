#pragma once
#include <cstdint>
#include "cdcMath/Math.h"
#include "cdcWorld/InstanceDrawable.h"
#include "Billboard.h"
#include "PackedVector.h"

namespace cdc { class TextureMap; class CommonMaterial; };

struct LensFlareAndCoronaExtraData {
	float float0;
	uint32_t numBillboards; // 4
	Billboard *billboards; // 8
	float floatC;
	PackedBytesAndScale packedVector; // 10
	float float18[3];
	uint32_t modelMode; // 24, 0 = billboards only
	uint32_t dword28;
	uint32_t dword2C;
	uint32_t dword30;
	uint32_t dword34;
	uint32_t dword38;
	uint32_t dword3C;
	uint32_t dword40;
	uint8_t gap44[28];
	uint32_t mode[8]; // 60
	PackedBytesAndScale packedVectors[4]; // 80
	cdc::Vector plainVectors[4]; // A0
	cdc::TextureMap *texture[4]; // E0
	cdc::CommonMaterial *material; // F0
	uint8_t gapF4[0x30];
	uint8_t byte124;
	uint8_t gap125[3];
	uint32_t dword128;
	uint32_t dword12C;
	uint32_t dword130;
	uint8_t byte134;
	uint8_t gap135[3];
	float float138;
	float float13C;
	uint8_t byte140;
	uint8_t gap141[3];
	float float144;
	float float148;
	float float14C;
	uint8_t byte150;
	float float154;
	uint32_t dword158;
	uint8_t byte15C;
	float offset[3]; // 160
};

static_assert(sizeof(LensFlareAndCoronaExtraData) == 0x16C);

class LensFlareAndCoronaID : public cdc::InstanceDrawable {
public:
	LensFlareAndCoronaID(Instance*);
	void GetBoundingVolume(cdc::BasicCullingVolume*) override;
	void draw(cdc::Matrix *, float) override;

	static cdc::Vector calcInstanceParamRow(
		uint32_t mode,
		cdc::Vector *vec0,
		cdc::Vector *vec1,
		cdc::Vector *vec2,
		cdc::Vector *vec3,
		cdc::Vector *vec4,
		cdc::Vector *vec5,
		cdc::Vector *vec6,
		cdc::Vector *vec7,
		cdc::Vector *scale,
		cdc::Vector *vec8,
		cdc::Vector *vec9,
		cdc::Vector *vec10,
		cdc::Vector *vec11
	);
};

void hackCalcInstanceParams(LensFlareAndCoronaExtraData *extra, cdc::Matrix *matrix, cdc::Matrix *invView, cdc::Vector4 *instanceParams);

