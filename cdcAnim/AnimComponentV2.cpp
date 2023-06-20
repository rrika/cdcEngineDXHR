#include "AnimComponentV2.h"
#include "cdcWorld/cdcWorldTypes.h" // for dtp::Model*

namespace cdc {

AnimComponentV2::AnimComponentV2(Instance *instance) :
	instance(instance)
{
	// TODO
}

void AnimComponentV2::Init(dtp::Model *model) {
	// TODO
	this->model = model;
}

}
