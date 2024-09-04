#include "DeferredRenderingObject.h"
#include "rendering/CommonMaterial.h"
#include "rendering/CommonScene.h"
#include "rendering/CommonRenderDevice.h"
#include "rendering/Culling/BasicPrimitives_inlines.h"
#include "rendering/PCDX11RenderModelInstance.h"
#include "cdcSys/Assert.h"
#include "cdcMath/MatrixInlines.h"
#include "cdcWorld/Instance.h"

using namespace cdc;

static bool cachedMatrixValid[16];
static Matrix cachedMatrix[16];
static uint32_t lastFrame = 0xffffffff;

// probably like this:
// 0 object space
// 1 world space
// 2 camera space
// 3 view space

static void cachedMatrixMulChain_setup(Matrix *objToWorld, Matrix *worldToObj) {

	cachedMatrixValid[2] = false;  // 0 2
	cachedMatrixValid[3] = false;  // 0 3
	cachedMatrixValid[8] = false;  // 2 0
	cachedMatrixValid[12] = false; // 3 0

	cachedMatrix[1] = *objToWorld;
	cachedMatrix[4] = *worldToObj;
	cachedMatrixValid[1] = true;  // 0 1
	cachedMatrixValid[4] = true;  // 1 0

	uint32_t thisFrame = g_renderDevice->getSceneCreationCount();
	if (lastFrame != thisFrame) {
		lastFrame = thisFrame;

		// fill in the identity matrices
		for (uint32_t i=0; i<4; i++) {
			cachedMatrix[5 * i] = identity4x4;
			cachedMatrixValid[5 * i] = true;
		}

		RenderViewport *viewport = g_renderDevice->getCurViewport();
		cachedMatrix[9] = viewport->viewMatrix; // 2 1
		cdc::OrthonormalInverse3x4(&cachedMatrix[6], cachedMatrix[9]); // 1 2

		CommonScene *scene = g_renderDevice->getScene();
		cachedMatrix[11] = scene->getProjectMatrix(); // 2 3
		// TODO invert [11] into [14] // 3 2

		cachedMatrix[7] = cachedMatrix[11] * cachedMatrix[6]; // 1 3
		// TODO cachedMatrix[13] = cachedMatrix[9] * cachedMatrix[14]; // 3 1


		cachedMatrixValid[6] = true;
		cachedMatrixValid[7] = true;
		cachedMatrixValid[9] = true;
		cachedMatrixValid[11] = true;
		// cachedMatrixValid[13] = true;
		// cachedMatrixValid[14] = true;
	}
}

static Matrix *cachedMatrixMulChain_eval(int p, int q) {
	uint32_t index = 4*p + q;
	if (cachedMatrixValid[index] == false) {
		Matrix *lhs, *rhs;
		if (p < q) {
			if (p+1 == q) { FatalError("infinite loop"); return nullptr; }
			lhs = cachedMatrixMulChain_eval(p+1, q);
			rhs = cachedMatrixMulChain_eval(p, p+1);
		} else {
			if (p == q+1) { FatalError("infinite loop"); return nullptr; }
			lhs = cachedMatrixMulChain_eval(q+1, q);
			rhs = cachedMatrixMulChain_eval(p, q+1);
		}

		cachedMatrix[index] = *lhs * *rhs; // matrix multiply
		cachedMatrixValid[index] = true;
	}

	return &cachedMatrix[index];
}

Vector DeferredRenderingObject::Drawable::calcInstanceParamRow(
	DeferredRenderingInstanceParam param,
	Vector *vec0,
	Vector *vec1,
	Vector *vec2,
	Vector *vec3,
	Vector *vec4,
	Vector *vec5,
	Vector *vec6,
	Vector *vec7,
	Vector *scale,
	float value9
) {
	// for deferred_fast_omni_diffuse it's like this
	// (taken from det_city_limb_clinic.drm section 0)
	// 3f250: 00000408
	// 3f254: 00ceecd8 packedVectors[0].packed
	// 3f258: 3f19999a packedVectors[0].scale
	// 3f25c: ff000000 packedVectors[1].packed
	// 3f260: 3f800000 packedVectors[1].scale
	// 3f264: ff000000 packedVectors[2].packed
	// 3f268: 3f800000 packedVectors[2].scale
	// 3f26c: ffffffff packedVectors[3].packed
	// 3f270: 3f800000 packedVectors[3].scale
	// 3f274: 3f800000 00000000 00000000 00000000 plainVectors[0]
	// 3f284: 00000000 3f800000 00000000 00000000 plainVectors[1]
	// 3f294: 00000000 00000000 3f800000 00000000 plainVectors[2]
	// 3f2a4: 00000000 00000000 00000000 3f800000 plainVectors[3]
	// 3f2b4: 00000000
	// 3f2b8: 00000000
	// 3f2bc: 00000000
	// 3f2c0: 00000000
	// 3f2c4: 00000000
	// 3f2c8: 43610000
	// 3f2cc: 00000000
	// 3f2d0: 00000000
	// 3f2d4: 3f800000
	// 3f2d8: 42b40000
	// 3f2dc: 00000000
	// 3f2e0: 459426ad
	// 3f2e4: 44c588e7
	// 3f2e8: 00000000
	// 3f2ec: 454588e7
	// 3f2f0: 44c588e7
	// 3f2f4: 3f000000
	// 3f2f8: c6200000
	// 3f2fc: 00000000
	// 3f300: pointer ->
	// 3f304: 00000000
	// 3f308: 0002010d instanceParamRow[0] mode = 13  P=1 Q=2   viewspace position
	// 3f30c: 00040400 instanceParamRow[1] mode =  0  no xform  color
	// 3f310: 00040409 instanceParamRow[2] mode =  9  no xform  inverse radius
	// 3f314: 00040404 instanceParamRow[3] mode =  4  no xform  light scale
	// 3f318: 00040408 instanceParamRow[4] mode =  8  no xform  unknown
	// 3f31c: 00040424 $... instanceParamRow[5] skipped
	// 3f320: 00040424 $... instanceParamRow[6] skipped
	// 3f324: 00040424 $... instanceParamRow[7] skipped

	Vector v;
	int p, q, row;

	switch (param.mode) {

	case 0: v = *vec0; break;
	case 1: v = *vec1; break;
	case 2: v = *vec2; break;
	case 3: v = *vec3; break;
	case 4: v = *vec4; break;
	case 5: v = *vec5; break;
	case 6: v = *vec6; break;
	case 7: v = *vec7; break;

	case 8: v = {scale->x, scale->y, scale->z, 1.0f}; break;
	case 9: v = {1.0f/scale->x, 1.0f/scale->y, 1.0f/scale->z, 1.0f}; break;

	case 10: p=0; q=1; row=0; goto matrix_row;
	case 11: p=0; q=1; row=1; goto matrix_row;
	case 12: p=0; q=1; row=2; goto matrix_row;
	case 13: p=0; q=1; row=3; goto matrix_row;

	case 14: p=1; q=2; row=0; goto matrix_row;
	case 15: p=1; q=2; row=1; goto matrix_row;
	case 16: p=1; q=2; row=2; goto matrix_row;
	case 17: p=1; q=2; row=3; goto matrix_row;

	case 18: p=2; q=3; row=0; goto matrix_row;
	case 19: p=2; q=3; row=1; goto matrix_row;
	case 20: p=2; q=3; row=2; goto matrix_row;
	case 21: p=2; q=3; row=3; goto matrix_row;

	case 22: p=1; q=0; row=0; goto matrix_row;
	case 23: p=1; q=0; row=1; goto matrix_row;
	case 24: p=1; q=0; row=2; goto matrix_row;
	case 25: p=1; q=0; row=3; goto matrix_row;

	case 26: p=2; q=1; row=0; goto matrix_row;
	case 27: p=2; q=1; row=1; goto matrix_row;
	case 28: p=2; q=1; row=2; goto matrix_row;
	case 29: p=2; q=1; row=3; goto matrix_row;

	case 30: p=3; q=2; row=0; goto matrix_row;
	case 31: p=3; q=2; row=1; goto matrix_row;
	case 32: p=3; q=2; row=2; goto matrix_row;
	case 33: p=3; q=2; row=3; goto matrix_row;

	matrix_row: {
		Matrix *m = cachedMatrixMulChain_eval(p, q);
		if (!m)
			FatalError("infinite loop requesting %d %d", p, q);
		v = {m->m[row][0], m->m[row][1], m->m[row][2], m->m[row][3]};
		break;
	}

	case 34:
	case 35:
	default:
		// FatalError("not implemented mode=%d", param.mode);
		return {};
	}

	if (param.matrixP != 4 && param.matrixQ != 4) {
		Matrix *m = cachedMatrixMulChain_eval(param.matrixP, param.matrixQ);
		if (param.multiplyMode == 0) {
			v = (*m) * Vector4{v};

		} else {
			// Matrix3x3 multiplication
			v = {
				m->m[0][0] * v.x + m->m[1][0] * v.y + m->m[2][0] * v.z,
				m->m[0][1] * v.x + m->m[1][1] * v.y + m->m[2][1] * v.z,
				m->m[0][2] * v.x + m->m[1][2] * v.y + m->m[2][2] * v.z,
				m->m[0][3] * v.x + m->m[1][3] * v.y + m->m[2][3] * v.z,
			};

			if (param.multiplyMode == 2)
				v = v.Normalize3();
		}
	}

	return v;
}

void hackCalcInstanceParams(DeferredRenderingExtraData *extra, Matrix *m, Vector4 *instanceParams) {
	Vector unpacked0 = extra->packedVectors[0].unpack();
	Vector unpacked1 = extra->packedVectors[1].unpack();
	Vector unpacked2 = extra->packedVectors[2].unpack();
	Vector unpacked3 = extra->packedVectors[3].unpack();
	Vector scale {1.0f, 1.0f, 1.0f, 0.0f};

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

	if (!instanceParams)
		return;

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

	cachedMatrixMulChain_setup(&objToWorld, &worldToObj);

	for (uint32_t i=0; i<8; i++) {
		auto& param = extra->params[i];
		if (param.mode != 36) {
			instanceParams[i] = Vector4{DeferredRenderingObject::Drawable::calcInstanceParamRow(
				param,
				&unpacked0,
				&unpacked1,
				&unpacked2,
				&unpacked3,
				&extra->plainVectors[0],
				&extra->plainVectors[1],
				&extra->plainVectors[2],
				&extra->plainVectors[3],
				&scale,
				extra->float88
			)};
		}
	}
}

DeferredRenderingObject::Drawable::Drawable(Instance *instance) :
	cdc::InstanceDrawable(instance)
{
	// TODO
}

void DeferredRenderingObject::Drawable::GetBoundingVolume(cdc::BasicCullingVolume *volume) {

	auto *extra = (DeferredRenderingExtraData*)m_instance->introData;
	float *scale = extra->scale;
	bool uniform = extra->scaleModeE1 == 1;
	Matrix scaleMatrix {
		scale[0], 0, 0, 0,
		0, scale[uniform?0:1], 0, 0,
		0, 0, scale[uniform?0:2], 0,
		0, 0, 0, 1
	};

	InstanceDrawable::GetBoundingVolume(volume);
	volume->m_data.box.Transform(scaleMatrix); // assume it's a box
}

void DeferredRenderingObject::Drawable::draw(cdc::Matrix *matrix, float arg) {
	// HACK
	auto matrixCopy = *matrix;
	auto *deferredExtraData = (DeferredRenderingExtraData*)m_instance->introData;
	auto modelIndex = m_instance->GetMeshComponent().GetCurrentRenderModelIndex();
	auto *rmi = static_cast<cdc::PCDX11RenderModelInstance*>(m_renderModelInstances[modelIndex]);

	rmi->baseMask = 0x2000; // deferred lighting
	rmi->setMaterial(~0u, static_cast<cdc::CommonMaterial*>(deferredExtraData->material));
	hackCalcInstanceParams(deferredExtraData, &matrixCopy, rmi->ext->instanceParams);

	// patch textures (even though this render model is shared between instances)
	cdc::PersistentPGData *ppg = rmi->getPersistentPGData();
	if (deferredExtraData->texture[0]) ppg->sub10.pInstanceTextures[0] = deferredExtraData->texture[0];
	if (deferredExtraData->texture[1]) ppg->sub10.pInstanceTextures[1] = deferredExtraData->texture[1];
	if (deferredExtraData->texture[2]) ppg->sub10.pInstanceTextures[2] = deferredExtraData->texture[2];
	if (deferredExtraData->texture[3]) ppg->sub10.pInstanceTextures[3] = deferredExtraData->texture[3];

	InstanceDrawable::draw(&matrixCopy, arg);
}
