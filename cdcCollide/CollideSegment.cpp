#include "collideMB.h"
#include "cdcMath/Math.h"
#include "cdcMath/VectorInlines.h"

namespace cdc {

CollideCode CollideSegmentAndAlignedBox( // line 30
	float& lambda,
	Vector3Arg s,
	Vector3Arg d,
	BBox& bbox
) {
	// TODO
}

CollideCode CollideSegmentAndTri( // line 230
	float *lambda,
	cdc::Vector3 *normal,
	Vector3Arg s,
	Vector3Arg d,
	MTriangle const& tri
) {
	auto v0 = tri.v0;
	auto v10 = tri.v1-tri.v0;
	auto v20 = tri.v2-tri.v0;
	auto n = Cross3(v10, v20);
	float impact = -Dot3(d, n);
	if (impact > 0.00001192092895507812) {
		auto sv0 = s-v0;
		float offset = Dot3(sv0, n);
		if (offset >= 0.f && offset <= impact * *lambda) {
			auto drift = Cross3(sv0, d);
			auto drift1 = Dot3(drift, v10);
			auto drift2 = Dot3(drift, v20);
			if (drift2 < -0.00001192092895507812 * impact)
				return NO_HIT;
			if (-drift1 >= -0.00001192092895507812 * impact && impact * 1.000011920928955 >= drift2-drift1) {

				*lambda = offset / impact;
				*normal = {n.Normalize3()};
				return HIT;
			}
		}
	}
	return NO_HIT;
}

}
