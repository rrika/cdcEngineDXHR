#include "LensFlareAndCoronaID.h"

using namespace cdc;

Vector LensFlareAndCoronaID::calcInstanceParamRow(
	uint32_t mode,
	Vector *vec0,
	Vector *vec1,
	Vector *vec2,
	Vector *vec3,
	Vector *vec4,
	Vector *vec5,
	Vector *vec6,
	Vector *vec7,
	Vector *scale,
	Vector *vec8,
	Vector *vec9,
	Vector *vec10,
	Vector *vec11
) {
	switch (mode) {
	case 0: return *vec0;
	case 1: return *vec1;
	case 2: return *vec2;
	case 3: return *vec3;

	case 4: return *vec9;

	case 5: return *vec4;
	case 6: return *vec5;
	case 7: return *vec6;
	case 8: return *vec7;

	case 9: return {scale->x, scale->y, scale->z, 1.0f};
	case 10: return {1.0f/scale->x, 1.0f/scale->y, 1.0f/scale->z, 1.0f};

	case 11: return {vec8->x, vec8->y, vec8->z, 1.0f};
	case 12: return *vec10;
	case 13: return *vec11;

	default:
		return {};
	}
}

void hackCalcInstanceParams(LensFlareAndCoronaExtraData *extra, Matrix *m, Matrix *invView, Vector4 *instanceParams) {
	Vector unpacked0 = extra->packedVectors[0].unpack();
	Vector unpacked1 = extra->packedVectors[1].unpack();
	Vector unpacked2 = extra->packedVectors[2].unpack();
	Vector unpacked3 = extra->packedVectors[3].unpack();
	Vector scale {1.0f, 1.0f, 1.0f, 0.0f};

	/*

	scale.x = extra->scale[0];
	scale.y = extra->scale[1];
	scale.z = extra->scale[2];

	if (extra->scaleModeE1 == 1) {
		scale.y = scale.x;
		scale.z = scale.x;
	}

	Matrix objToWorld = *m;
	Matrix worldToObj;
	cdc::OrthonormalInverse3x4(&worldToObj, objToWorld);

	objToWorld.m[0][0] *= scale.x;
	objToWorld.m[0][1] *= scale.x;
	objToWorld.m[0][2] *= scale.x;
	objToWorld.m[1][0] *= scale.y;
	objToWorld.m[1][1] *= scale.y;
	objToWorld.m[1][2] *= scale.y;
	objToWorld.m[2][0] *= scale.z;
	objToWorld.m[2][1] *= scale.z;
	objToWorld.m[2][2] *= scale.z;

	// write the modified matrix back out
	*m = objToWorld;


	Vector invScale {1.0f/scale.x, 1.0f/scale.y, 1.0f/scale.z, 0.0f};
	worldToObj.m[0][0] *= invScale.x;
	worldToObj.m[0][1] *= invScale.y;
	worldToObj.m[0][2] *= invScale.z;
	worldToObj.m[1][0] *= invScale.x;
	worldToObj.m[1][1] *= invScale.y;
	worldToObj.m[1][2] *= invScale.z;
	worldToObj.m[2][0] *= invScale.x;
	worldToObj.m[2][1] *= invScale.y;
	worldToObj.m[2][2] *= invScale.z;
	*/

	Vector dummy{0.5f, 0.5f, 0.5f, 0.5f};

	Vector invView0 { invView->m[0][0], invView->m[0][1], invView->m[0][2], invView->m[0][3] };
	Vector invView1 { invView->m[1][1], invView->m[1][1], invView->m[1][2], invView->m[1][3] };

	Vector pos { m->m[3][0], m->m[3][1], m->m[3][2], m->m[3][3] };
	Vector offset { extra->offset[0], extra->offset[1], extra->offset[2], 0.0f };
	Vector pos2 = pos + offset;
	// no idea what this accomplishes but whatever
	Vector vec10 = pos2 + (invView0 + invView1) * (extra->float154 * -0.5);
	Vector vec11 = pos2 + (invView0 + invView1) * (extra->float154 *  0.5);

	for (uint32_t i=0; i<8; i++) {
		uint32_t mode = extra->mode[i];
		if (mode != 14) {
			instanceParams[i] = Vector4{LensFlareAndCoronaID::calcInstanceParamRow(
				mode,
				&unpacked0,
				&unpacked1,
				&unpacked2,
				&unpacked3,
				&extra->plainVectors[0],
				&extra->plainVectors[1],
				&extra->plainVectors[2],
				&extra->plainVectors[3],
				&scale,
				&dummy,
				&dummy,
				&vec10,
				&vec11
			)};
		}
	}
}