#pragma once
#include "cdcWorld/InstanceComponent.h"

namespace cdc {

class CollideComponent : public InstanceComponent {
public:
	void EnableNoCollide();
	void DisableNoCollide();
	void SetupHModelListForSingleModel();
	void SetupHModelList();
	void UpdateCollideInternal();

	// void SaveState(BinaryWriter*) override;
	// void RestoreState(BinaryReader*) override;
};

}