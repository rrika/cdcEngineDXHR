#pragma once
#include "Math.h"

namespace cdc {

inline Quat operator*(QuatArg quatA, QuatArg quatB) { // line 63
	return {
		quatA.w*quatB.x + quatA.x*quatB.w + quatA.y*quatB.z - quatA.z*quatB.y, // i
		quatA.w*quatB.y - quatA.x*quatB.z + quatA.y*quatB.w + quatA.z*quatB.x, // j
		quatA.w*quatB.z + quatA.x*quatB.y - quatA.y*quatB.x + quatA.z*quatB.w, // k
		quatA.w*quatB.w - quatA.x*quatB.x - quatA.y*quatB.y - quatA.z*quatB.z  // 1
	};
}

void Quat::Normalize(){
	*this *= 1.f / sqrtf(x*x + y*y + z*z + w*w);
}

}
