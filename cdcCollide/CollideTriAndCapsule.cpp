#include "collideMB.h"

namespace cdc {

int32_t ComputeEdgeContact( // line 28
	CPoint *contacts,
	Vector3Arg s,
	Vector3Arg t,
	float radius,
	Vector3Arg v0,
	Vector3Arg v1,
	Vector3Arg v2,
	Vector3Arg triNormal
) {

}

int32_t CollideTriAndCapsule( // line 65
	CPoint *contacts,
	int32_t maxContacts,
	MTriangle const& tri,
	Vector3Arg s,
	Vector3Arg t,
	float radius,
	uint8_t adjacencyFlags,
	bool unknown
) {
	//
}

}
