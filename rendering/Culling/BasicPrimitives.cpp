#include "BasicPrimitives.h"
#include "BasicPrimitives_inlines.h"

namespace cdc {

void BasicCullingVolume::Transform(Matrix const& m) { // line 1087
	switch (m_type) {
		case kVolumeSphere:
			m_data.sphere.Transform(m);
			break;

		case kVolumeBox:
			m_data.box.Transform(m);
			break;

		default:
			break;
	}
}

}
