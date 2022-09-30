#pragma once
#include <cstdint>
#include "../../../../math/Math.h"

namespace cdc { class IMaterial; class ResolveObject; class IMFRefRuntimeInstanceBase; }

namespace dtp {

struct IMFRef {

	// struct InstanceParams { // 40
	// 	float m_instanceParam0[2]; // 0
	// 	float m_instanceParam1[4]; // 8
	// 	float m_instanceParam2[4]; // 18
	// 	float m_instanceParam3[4]; // 28
	// };

	struct FixedMatrix { // 47
		cdc::Matrix m_fixedTransform;
		// cdc::Vector3 m_bboxScale;
	};

	struct MaterialOverride { // 52
		uint32_t m_sourceMaterialID;
		cdc::IMaterial *m_pDestMaterial;
		// uint32_t m_streamLayer;
	};

	cdc::Matrix m_transform; // 0
	FixedMatrix *m_pFixedTransform; // 40
	const char *m_debugName; // 44
	uint32_t dtpID; // 48
	char *m_imfDRMName; // 4C
	uint32_t crc32; // 50
	uint8_t imfCounterCategory; // 54
	uint8_t byte55;
	uint8_t byte56;
	uint8_t byte57;
	uint32_t dword58;
	uint32_t dword5C;
	uint32_t dword60;
	uint32_t dword64;
	uint32_t dword68;
	uint32_t dword6C;
	uint32_t m_numOverrideMaterials; // 70
	MaterialOverride *m_overrideMaterials; // 74
	uint32_t dword78;
	uint32_t dword7C;
	uint32_t dword80;
	cdc::IMFRefRuntimeInstanceBase *m_pData; // 84
	cdc::ResolveObject *m_pResolveObject; // 88
	uint32_t engine_cellNamePtr; // 8C
};

}
