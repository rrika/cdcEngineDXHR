#include "AnimComponentV2.h"
#include "cdcAnim/AnimGraphFactory.h"
#include "cdcWorld/cdcWorldTypes.h" // for dtp::Model*
#include "cdcWorld/Instance.h"
#include "cdcWorld/Object.h"
#include "cdc/dtp/animgraphcommon.h"

namespace cdc {

AnimComponentV2::AnimComponentV2(Instance *instance) :
	instance(instance)
{
	// TODO
}

void AnimComponentV2::instantiate(dtp::Model *model) {
	// TODO
	this->model = model;
}

dtp::AnimGraphExt *AnimComponentV2::getExt(uint32_t index) {
	// TODO
	return &instance->object->dtpData->pAnimGraph->ext[index];
}

}
