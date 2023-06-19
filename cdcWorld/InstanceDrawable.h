#pragma once
#include <cstdint>
#include <vector>
#include "rendering/RenderModelInstance.h"
#include "cdcScene/IDrawable.h"
#include "cdcWorld/InstanceComponent.h"

namespace dtp {
	struct Model;
}

namespace cdc {

struct BasicCullingVolume;
struct Matrix;
class IMatrixState;

class MeshComponent : public InstanceComponent {
	Instance *m_instance; // 4
	uint8_t m_currentRenderModel = 0; // 8
	uint8_t m_currentBaseModel = 0; // 9
	// uint32_t dwordC;
	// uint32_t dword10;
	// uint32_t dword14;
	// uint32_t dword18;
	// uint32_t dword1C;

public:
	MeshComponent(Instance *instance) : m_instance(instance) {}
	uint8_t GetCurrentRenderModelIndex() { return m_currentRenderModel; }
	dtp::Model *GetModel();
	void SetModel(uint32_t index);
	void SetModel(uint32_t index, dtp::Model *baseModel);
};

class InstanceDrawable :
	public cdc::IDrawable,
	public InstanceComponent
{
public:
	Instance *m_instance; // 8

	std::vector<cdc::RenderModelInstance*> m_renderModelInstances; // C
	cdc::IMatrixState *m_pMatrixState; // 24
protected:
	InstanceDrawable *m_pPrevDirty = nullptr;
	InstanceDrawable *m_pNextDirty = nullptr;

public:
	// dirty list is processed by SceneLayer::Update/UpdateInstances
	static InstanceDrawable *s_pFirstDirty;
	static InstanceDrawable *s_pLastDirty;

	InstanceDrawable(Instance *i);

	void GetBoundingVolume(cdc::BasicCullingVolume*) override;
	void draw(cdc::Matrix *, float) override;
	bool GetBoundingSphere(Vector *pCenter, float *pRadius) override;
	bool GetBoundingBox(Vector *pMin, Vector *pMax) override;

	void AddToDirtyList();
	void RemoveFromDirtyList();
};

}
