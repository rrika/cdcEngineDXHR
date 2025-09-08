#pragma once
#include <cstdint>
#include "cdcMath/Math.h"
#include "cdcWorld/InstanceDrawable.h"
#include "PackedVector.h"

namespace cdc { class CommonMaterial; class TextureMap; }

struct DeferredRenderingInstanceParam {
	uint8_t mode;
	uint8_t matrixP;
	uint8_t matrixQ;
	uint8_t multiplyMode;
};

struct DeferredRenderingExtraData {
	uint8_t gap0[4];
	PackedBytesAndScale packedVectors[4]; // 4
	cdc::Vector plainVectors[4]; // 24
	cdc::TextureMap *texture[4]; // 64
	uint8_t gap74[4];
	float scale[3]; // 78
	float float84;
	float angle88;

	bool farFadeEnable; // 8C
	float farFadeStart; // 90
	float farFadeWidth; // 94

	bool nearFadeEnable; // 98
	float nearFadeStart; // 9C
	float nearFadeWidth; // A0
	float nearFadeLevel; // A4
	float floatA8;

	uint8_t gapAC[9];
	uint8_t sourceSpaceB5;
	uint8_t targetSpaceB6;
	uint8_t instanceParamFromMatrixB7;
	DeferredRenderingInstanceParam params[8]; // B8
	uint8_t byteD8;
	cdc::CommonMaterial *material; // DC
	uint8_t lightRatherThanCorona; // E0
	uint8_t scaleModeE1;
	uint8_t byteE2;
	uint8_t gapE3[49];
	uint32_t dword114;
	uint32_t dword118;

	float fadeByDistance(float dist);
};

struct DeferredRenderingObject {
	class Drawable : public cdc::InstanceDrawable {
	public:
		Drawable(Instance*);
		void GetBoundingVolume(cdc::BasicCullingVolume*) override;
		void draw(cdc::Matrix *, float) override;

		static cdc::Vector calcInstanceParamRow(
			DeferredRenderingInstanceParam param,
			cdc::Vector *vec0,
			cdc::Vector *vec1,
			cdc::Vector *vec2,
			cdc::Vector *vec3,
			cdc::Vector *vec4,
			cdc::Vector *vec5,
			cdc::Vector *vec6,
			cdc::Vector *vec7,
			cdc::Vector *vec8,
			float value9
		);
	};
};

void hackCalcInstanceParams(DeferredRenderingExtraData *extra, cdc::Matrix *matrix, cdc::Vector4 *instanceParams);
