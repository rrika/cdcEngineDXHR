#pragma once
#include <cstdint>
#include "cdcWorld/cdcWorldTypes.h"
#include "cdcWorld/InstanceComponent.h"

namespace cdc {

class CollideComponent : public InstanceComponent {
	uint32_t vtable;
	HModel *m_hModelList; // 4
	uint32_t m_animatedGeoms; // 8
	uint8_t padC[28];
	Instance *instance28;
	uint8_t byte2C;
	uint8_t byte2D;
	uint8_t byte2E;
	uint8_t byte2F;
	uint8_t pad30[4];
public:
	void UpdateCollideInternal();
	void SetupHModelList();
	void SetupHModelListForSingleModel();
	void EnableNoCollide();
	void DisableNoCollide();
	HModel *GetHModelList() { return m_hModelList; }

	// void SaveState(BinaryWriter*) override;
	// void RestoreState(BinaryReader*) override;
};

}