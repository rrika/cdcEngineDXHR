#pragma once
#include <cstdint>
#include "cdcMath/Math.h"

namespace cdc { class CommonMaterial; }

struct DeferredRenderingPackedBytesAndScale {
	uint32_t bytes;
	float scale;

	cdc::Vector unpack();
};

struct DeferredRenderingInstanceParam {
	uint8_t mode;
	uint8_t matrixP;
	uint8_t matrixQ;
	uint8_t multiplyMode;
};

struct DeferredRenderingExtraData {
	uint8_t gap0[4];
	DeferredRenderingPackedBytesAndScale packedVectors[4];
	cdc::Vector plainVectors[4];
	uint32_t dword64;
	uint32_t dword68;
	uint32_t dword6C;
	uint32_t dword70;
	uint8_t gap74[4];
	float scale[3]; // 78
	float float84;
	float float88;
	uint8_t byte8C;
	float float90;
	float float94;
	uint8_t byte98;
	float float9C;
	float floatA0;
	float floatA4;
	float floatA8;
	uint8_t gapAC[9];
	uint8_t sourceSpaceB5;
	uint8_t targetSpaceB6;
	uint8_t instanceParamFromMatrixB7;
	DeferredRenderingInstanceParam params[8]; // B8
	uint8_t byteD8;
	cdc::CommonMaterial *material; // DC
	uint8_t byteE0;
	uint8_t scaleModeE1;
	uint8_t byteE2;
	uint8_t gapE3[49];
	uint32_t dword114;
	uint32_t dword118;
};

struct DeferredRenderingObject {
	class Drawable /* : InstanceDrawable */ {
	public:
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
