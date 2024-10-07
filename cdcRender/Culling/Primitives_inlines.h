#pragma once
#include "Primitives.h"

namespace cdc {

inline bool Intersects(BasicCullingVolume const& a, CullingFrustum const& b) { // line 1434
	switch (a.m_type) {
		case kVolumeSphere:
			return Intersects(a.m_data.sphere, b);

		case kVolumeBox:
			return Intersects(a.m_data.box, b);

		case kVolumeNothing:
			return false;

		case kVolumeEverything:
			return true;

		default:
			return true;
	}
}

}

