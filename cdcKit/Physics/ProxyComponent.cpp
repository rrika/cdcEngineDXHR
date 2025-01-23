#include "cdcPhysics/Multibody.h"
#include "ProxyComponent.h"

namespace cdc {

void ProxyComponent::InitCommon(/*...*/) {
	MultibodySystem *ms;
	if (joint730)
		ms->DestroyJoint(joint730);
	if (joint730)
		ms->DestroyJoint(joint730);

}

}
