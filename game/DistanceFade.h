#pragma once

struct DistanceFade {
	bool farFadeEnable;
	float farFadeStart;
	float farFadeWidth;

	bool nearFadeEnable;
	float nearFadeStart;
	float nearFadeWidth;
	float nearFadeLevel;

	float eval(float dist);
};
