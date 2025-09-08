#pragma once
#include <cstdint>
#include "MaterialData.h"
#include "RenderMesh.h"
#include "RenderModelInstance.h"
#include "cdcMath/Math.h"

namespace cdc {

class PCDX11Material;

struct NonPersistentPGData {
	uint32_t dword0;   // = m_instanceFlags
	float float4;      // = m_sortOffset
	uint32_t mask8;    // = m_renderPassMask
	uint32_t dwordC;   // = m_shadowRegionMask
};

struct PersistentPGData {  // created in PCDX11RenderModel::resConstruct
	PCDX11Material *material;
	uint32_t dword4;
	uint32_t dword8;
	uint32_t dwordC;
	MaterialInstanceData sub10;
	uint32_t dword78; // probably also part of MaterialInstanceData
	uint32_t dword7C; // a subfunction of RenderModel::load does a memcpy of 0x70

	bool hide; // HACK
};

struct RenderModelInstanceData {
	Matrix projectOverride; // 0
	bool projectOverrideValid = false; // 40
	// TODO
	Vector4 instanceParams[8];
};

class CommonRenderModelInstance : public RenderModelInstance {
public:
	uint32_t baseMask; // 8
protected:
	RenderMesh *renderMesh;
	NonPersistentPGData *tab0Ext16; // 24
	PersistentPGData *tab0Ext128; // 2C
	RenderModelInstanceData *m_pCurrentInstanceData; // 34

public:
	CommonRenderModelInstance(RenderMesh *renderMesh) :
		renderMesh(renderMesh)
	{
		tab0Ext16 = renderMesh->getTab0Ext16();
		tab0Ext128 = renderMesh->getTab0Ext128();
		m_pCurrentInstanceData = nullptr;
	}
	~CommonRenderModelInstance() {
		delete m_pCurrentInstanceData;
	}

	RenderModelInstanceData *accessInstanceData() {
		if (m_pCurrentInstanceData == nullptr)
			m_pCurrentInstanceData = new RenderModelInstanceData;
		return m_pCurrentInstanceData;
	}

	void SetMaterial(uint32_t selector, IMaterial *material) override;

	void SetDrawBehindAll(bool depthLayer) override {
		ModelData *md = renderMesh->getMesh();
		md->flags.depthLayer = depthLayer;
	}

	void SetProjectionOverride(Matrix const *m) override {
		if (m_pCurrentInstanceData && m) {
			m_pCurrentInstanceData->projectOverride = *m;
			m_pCurrentInstanceData->projectOverrideValid = true; 
		}
	}

	PersistentPGData *getPersistentPGData() override { return tab0Ext128; } // TODO: confirm
	NonPersistentPGData *getNonPersistentPGData() override { return tab0Ext16; } // TODO: confirm
};

}
