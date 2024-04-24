#include <cstring>
#include "AnimPose.h"

namespace cdc {

void AnimPose::AllocSegs(uint32_t numSegments, Instance *instance, uint32_t) {
	// HACK
	if (!buffer)
		buffer = new AnimBuffer;

	if (buffer->segments)
		delete[] buffer->segments;

	buffer->numSegments = numSegments;
	buffer->segments = new AnimSegment[numSegments];
}

void AnimPose::ClearSegValues(float scale) {
	// TODO
	memset(buffer->segments, 0, sizeof(AnimSegment) * buffer->numSegments);
	clearedByFragment = false;
}

}