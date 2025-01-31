#pragma once
#include "collideMB.h"

namespace cdc {

inline bool TestAlignedBoxAndAlignedBox( // line 77
	Vector3Arg aMin,
	Vector3Arg aMax,
	Vector3Arg bMin,
	Vector3Arg bMax
) {
	return
		aMax.x > bMin.x &&
		aMax.y > bMin.y &&
		aMax.z > bMin.z &&
		aMin.x < bMax.x &&
		aMin.y < bMax.y &&
		aMin.z < bMax.z;
}

inline BBox CalcXformedBBox(BBox const& A, Matrix const& M) { // line 163
	// TODO
	return A;
}

}
