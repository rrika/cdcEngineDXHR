#pragma once
#include <cstdint>
#include "MaterialData.h"
#include "RenderMesh.h"
#include "RenderModelInstance.h"

namespace cdc {

class PCDX11Material;

struct MeshTab0Ext16 { // = cdc::NonPersistentPGData
	uint32_t dword0;   // = m_instanceFlags
	float float4;      // = m_sortOffset
	uint32_t mask8;    // = m_renderPassMask
	uint32_t dwordC;   // = m_shadowRegionMask
};

struct MeshTab0Ext128 { // = cdc::PersistentPGData
	PCDX11Material *material;
	uint32_t dword4;
	uint32_t dword8;
	uint32_t dwordC;
	MaterialInstanceData sub10;
	uint32_t dword78; // probably also part of MaterialInstanceData
	uint32_t dword7C; // a subfunction of RenderModel::load does a memcpy of 0x70
};

class CommonRenderModelInstance : public RenderModelInstance {
public:
	uint32_t baseMask; // 8
protected:
	RenderMesh *renderMesh;
	MeshTab0Ext16 *tab0Ext16; // 24
	MeshTab0Ext128 *tab0Ext128; // 2C
public:
	CommonRenderModelInstance(RenderMesh *renderMesh) :
		renderMesh(renderMesh)
	{
		tab0Ext16 = renderMesh->getTab0Ext16();
		tab0Ext128 = renderMesh->getTab0Ext128();
	}
};

}
