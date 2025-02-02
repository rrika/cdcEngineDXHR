#include "SweptSphere.h"
#include "cdcPhysics/MultibodySystem.h"

namespace cdc {

CollideCode CollideSweptSphereAndSphere( // line 12
	float& lambda,
	Vector3 *normal,
	MSphere const& sphere,
	Vector3Arg d,
	MSphere const& other
) {
	MSphere bigSphere {
		other.x,
		sphere.radius + other.radius
	};
	return CollideSegmentAndSphere(lambda, normal, sphere.x, d, bigSphere);
}

void CollidePhantoms(MultibodySystemImpl *mb) {
	// TODO
}

}
