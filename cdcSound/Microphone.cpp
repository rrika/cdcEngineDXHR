#include <cmath>
#include "Microphone.h"
#include "cdcMath/Math.h"
#include "cdcMath/MatrixInlines.h"
#include "cdcMath/VectorInlines.h"

namespace cdc {

Microphone g_microphone;

void Microphone::GetRelativePositionVector(
	SoundTypes::Controls3d *controls3d,
	Vector4 *pVector,
	float *pDistance)
{
	Vector3 position {
		controls3d->position[0],
		controls3d->position[1],
		controls3d->position[2]};

	Vector relativePosition = m_viewMatrix * position;

	float distance = sqrtf(Dot3(relativePosition, relativePosition));
	relativePosition.SafeNormalize3();

	*pVector = {relativePosition};
	*pDistance = distance;
}

}
