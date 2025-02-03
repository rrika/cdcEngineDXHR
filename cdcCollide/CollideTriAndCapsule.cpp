#include "collideMB.h"
#include "CollideUtils.h"

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
	Vector3 cp1, cp2;
	float distSqr = SegmentToSegmentDistance(cp1, cp2, s, t, v0, v1, nullptr, nullptr);
	if (radius * radius < distSqr)
		return 0;

	Vector3 normal = (cp2 - cp1);
	if (normal.LenSquared() < 1e-6f)
		normal = triNormal;
	normal.Normalize3();
	contacts->position = cp2 - normal * radius;
	contacts->normal = normal;
	contacts->separation = sqrtf(distSqr) - radius;
	return 1;
}

int32_t CollideTriAndCapsule( // line 65
	CPoint *contacts,
	int32_t maxContacts,
	MTriangle const& tri,
	Vector3Arg s,
	Vector3Arg t,
	float radius,
	uint8_t adjacencyFlags,
	bool reqCenterOfMassAbove // no hit if center of mass is below triangle
) {
	Vector3 d = t-s;
	if (d.LenSquared() <= 1e-6f)
		return CollideTriAndSphere(contacts, s, radius, tri, adjacencyFlags, reqCenterOfMassAbove);

	float kInflate = 1.0000119f;
	Vector3 weakAvg = (tri.v0 + tri.v1 + tri.v2) * (kInflate / 3);
	Vector3 h0 = tri.v0 * kInflate - weakAvg;
	Vector3 h1 = tri.v1 * kInflate - weakAvg;
	Vector3 h2 = tri.v2 * kInflate - weakAvg;

	Vector h10 = h1 - h0;
	Vector h21 = h2 - h1;
	Vector h02 = h0 - h2;

	Vector3 normal = {Cross3(h02, h10)}; // confirm sign, also why do this after inflation?
	if (normal.IsZero3())
		return 0;
	normal.Normalize3();

	float centerLevel = Dot3(normal, (t+s) * 0.5 - h0);

	if (reqCenterOfMassAbove && centerLevel < 0.0)
		return 0;

	// bounding box test
	Vector3 r = {radius, radius, radius, 0.f};
	Vector triMin = CompMin4(h0, CompMin4(h1, h2));
	Vector triMax = CompMax4(h0, CompMax4(h1, h2));
	Vector capMin = CompMin4(s, t)-r;
	Vector capMax = CompMax4(s, t)+r;
	Vector bboxOverlap = CompMax4(triMin - capMax, capMin - triMax);
	if (bboxOverlap.x > 0.0 || bboxOverlap.y > 0.0 || bboxOverlap.z > 0.0)
		return 0;

	float sLevel = Dot3(normal, s - h0);
	float tLevel = Dot3(normal, t - h0);

	// start and end sphere below triangle => no intersection
	if (!reqCenterOfMassAbove && (sLevel + radius < 0.0 && tLevel + radius < 0.0))
		return 0;

	float sSeparation = sLevel - radius;
	float tSeparation = tLevel - radius;

	if (!(sSeparation <= 0.f || tSeparation <= 0.f))
		return 0;

	// s sphere center is inside triangle
	Vector3 sCheck = {
		Dot3(Cross3(h10, s - h0), normal),
		Dot3(Cross3(h21, s - h1), normal),
		Dot3(Cross3(h02, s - h2), normal)
	};

	// t sphere center is inside triangle
	Vector3 tCheck = {
		Dot3(Cross3(h10, t - h0), normal),
		Dot3(Cross3(h21, t - h1), normal),
		Dot3(Cross3(h02, t - h2), normal)
	};

	bool insideS = sCheck.x > 0.f && sCheck.y > 0.f && sCheck.z > 0.f;
	bool insideT = tCheck.x > 0.f && tCheck.y > 0.f && tCheck.z > 0.f;

	uint32_t numContacts = 0;

	if (insideS && sSeparation < 0.f) {
		contacts[numContacts].position = s - normal * radius;
		contacts[numContacts].normal = normal;
		contacts[numContacts].separation = sSeparation;
		numContacts++;
	}

	if (numContacts == maxContacts)
		return numContacts;

	if (insideT && tSeparation < 0.f) {
		contacts[numContacts].position = t - normal * radius;
		contacts[numContacts].normal = normal;
		contacts[numContacts].separation = tSeparation;
		numContacts++;
	}

	if (insideS && insideT)
		return numContacts;

	if (numContacts == maxContacts)
		return numContacts;

	if (adjacencyFlags & 8)
		numContacts += ComputeEdgeContact(contacts + numContacts, s, t, radius, h0, h1, h2, normal);

	if (numContacts == 2 || numContacts == maxContacts)
		return numContacts;

	if (adjacencyFlags & 0x10)
		numContacts += ComputeEdgeContact(contacts + numContacts, s, t, radius, h1, h2, h0, normal);

	if (numContacts == 2 || numContacts == maxContacts)
		return numContacts;

	if (adjacencyFlags & 0x20)
		numContacts += ComputeEdgeContact(contacts + numContacts, s, t, radius, h2, h0, h1, normal);

	return numContacts;
}

}
