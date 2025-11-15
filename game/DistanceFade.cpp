#include "DistanceFade.h"

float DistanceFade::eval(float dist) {
	float factor = 1.f;
	if (farFadeEnable && farFadeStart + farFadeWidth < dist)
		return 0.f;
	if (nearFadeEnable && nearFadeStart - nearFadeWidth > dist && nearFadeLevel == 0.f)
		return 0.f;
	if (farFadeEnable && farFadeStart < dist) {
		float f = 1.0 - (dist - farFadeStart) / farFadeWidth;
		if (f <= 0.f) f = 0.f;
		if (f >= 1.f) f = 1.f;
		factor = f;
	}
	if (nearFadeEnable && nearFadeStart > dist) {
		float f = (dist - (nearFadeStart - nearFadeWidth)) / nearFadeWidth;
		if (f <= 0.f) f = 0.f;
		if (f >= 1.f) f = 1.f;
		f = (1.0 - nearFadeLevel) * f + nearFadeLevel;
		factor *= f;
	}
	return factor;
}
