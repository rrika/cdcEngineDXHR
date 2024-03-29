#pragma once
#include <cstdint>
#include "cdcMath/Math.h"
#include "cdcSound/cdcSound.h"

namespace cdc {

class Microphone {
public:
	Vector m_position; // 0
	Matrix m_viewMatrix; // HACK

	void GetRelativePositionVector(
		SoundTypes::Controls3d *controls3d,
		Vector4 *pVector,
		float *pDistance);
};

extern Microphone g_microphone;

}
