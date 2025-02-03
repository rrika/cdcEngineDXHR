#include "collideMB.h"
#include "CollideUtils.h"
#include "cdcMath/VectorInlines.h"

namespace cdc {

// TL. Note: Jesus christ...

int32_t CollideTriAndSphere(
	CPoint *contacts,
	Vector3Arg p,
	float radius,
	MTriangle const& tri,
	uint8_t adjacencyFlags,
	bool reqCenterOfMassAbove
) {
	Vector3 weakAvg = {(tri.v0 + tri.v1 + tri.v2) * 0.0000039736433f};
	Vector3 h0 = tri.v0 * 1.0000119f - weakAvg;
	Vector3 h1 = tri.v1 * 1.0000119f - weakAvg;
	Vector3 h2 = tri.v2 * 1.0000119f - weakAvg;

	Vector3 h10 = h1 - h0;
	Vector3 h21 = h2 - h1;
	Vector3 h02 = h0 - h2;

	Vector3 triNormal = {Cross3(h02, h10)};
	triNormal.Normalize3();
	contacts->tnormal = triNormal;
	// contacts->tnormal = {0.f, 0.f, 0.f};

	// below triangle?
	float centerLevel = Dot3(triNormal, p - h0);
	if (reqCenterOfMassAbove && centerLevel < 0.0f)
		return 0;

	// too far from triangle?
	float separation = fabsf(centerLevel) - radius;
	if (separation > radius)
		return 0;

	// check h1 corner
	float dot_h10_h1_minus_p = Dot3(h1 - p, h10);
	float dot_h21_p_minus_h1 = Dot3(p - h1, h21);

	if (dot_h10_h1_minus_p <= 0.f && dot_h21_p_minus_h1 <= 0.0f) {
		if (adjacencyFlags & 0x18) {
			float lenSquared = (p-h1).LenSquared();
			if (lenSquared <= radius * radius) {
				contacts->position = h1;
				contacts->separation = sqrtf(lenSquared) - radius;
				if (lenSquared > 0.00000011920929f) {
					Vector3 normal = (p-h1);
					normal.Normalize3();
					contacts->normal = normal;
				} else {
					contacts->normal = triNormal;
				}
				return 1;
			}
		}
		return 0;
	}

	// check h2 corner
	float dot_h21_h2_minus_p = Dot3(h2 - p, h21);
	float dot_h02_p_minus_h2 = Dot3(p - h2, h02);

	if (dot_h21_h2_minus_p <= 0.f && dot_h02_p_minus_h2 <= 0.f) {
		if (adjacencyFlags & 0x30) {
			float lenSquared = (p-h2).LenSquared();
			if (lenSquared <= radius * radius) {
				contacts->position = h2;
				contacts->separation = sqrtf(lenSquared) - radius;
				if (lenSquared > 0.00000011920929f) {
					Vector3 normal = (p-h2);
					normal.Normalize3();
					contacts->normal = normal;
				} else {
					contacts->normal = triNormal;
				}
				return 1;
			}
		}
		return 0;
	}

	// check h0 corner
	float dot_h10_p_minus_h0 = Dot3(p - h0, h10);
	float dot_h02_h0_minus_p = Dot3(h0 - p, h02);

	if (dot_h10_p_minus_h0 <= 0.f && dot_h02_h0_minus_p <= 0.f) {
		if (adjacencyFlags & 0x28) {
			float lenSquared = (p-h0).LenSquared();
			if (lenSquared <= radius * radius) {
				contacts->position = h0;
				contacts->separation = sqrtf(lenSquared) - radius;
				if (lenSquared > 0.00000011920929f) {
					Vector3 normal = (p-h0);
					normal.Normalize3();
					contacts->normal = normal;
				} else {
					contacts->normal = triNormal;
				}
				return 1;
			}
		}
		return 0;
	}

	float inside10 = Dot3(Cross3(h10, p - h0), triNormal);
	float inside21 = Dot3(Cross3(h21, p - h1), triNormal);
	float inside02 = Dot3(Cross3(h02, p - h2), triNormal);

	// check h0-h1 edge
	if (inside10 <= 0.f) {
		if (dot_h10_h1_minus_p >= 0.f && dot_h10_p_minus_h0 >= 0.f) {
			if (adjacencyFlags & 8) {
				float lambda = dot_h10_p_minus_h0 / (dot_h10_p_minus_h0 + dot_h10_h1_minus_p);
				Vector3 cp = h0 + h10 * lambda;
				Vector3 dist = p - cp;
				float distSqr = dist.LenSquared();
				if (radius * radius >= distSqr) {
					if (distSqr < 1e-6f)
						contacts->normal = triNormal;
					else
						contacts->normal = dist.UnitVec();
					contacts->position = cp;
					contacts->separation = sqrtf(distSqr) - radius;
					return 1;
				}
			}
		}
		//contacts->tnormal = h10.UnitVec();
	}

	// check h1-h2 edge
	if (inside21 <= 0.f) {
		if (dot_h21_h2_minus_p >= 0.f && dot_h21_p_minus_h1 >= 0.f) {
			if (adjacencyFlags & 0x10) {
				float lambda = dot_h21_p_minus_h1 / (dot_h21_p_minus_h1 + dot_h21_h2_minus_p);
				Vector3 cp = h1 + h21 * lambda;
				Vector3 dist = p - cp;
				float distSqr = dist.LenSquared();
				if (radius * radius >= distSqr) {
					if (distSqr < 1e-6f)
						contacts->normal = triNormal;
					else
						contacts->normal = dist.UnitVec();
					contacts->position = cp;
					contacts->separation = sqrtf(distSqr) - radius;
					return 1;
				}
			}
		}
		//contacts->tnormal = h21.UnitVec();
	}

	// check h2-h0 edge
	if (inside02 <= 0.f) {
		if (dot_h02_h0_minus_p >= 0.f && dot_h02_p_minus_h2 >= 0.f) {
			if (adjacencyFlags & 0x20) {
				float lambda = dot_h02_p_minus_h2 / (dot_h02_p_minus_h2 + dot_h02_h0_minus_p);
				Vector3 cp = h2 + h02 * lambda;
				Vector3 dist = p - cp;
				float distSqr = dist.LenSquared();
				if (radius * radius >= distSqr) {
					if (distSqr < 1e-6f)
						contacts->normal = triNormal;
					else
						contacts->normal = dist.UnitVec();
					contacts->position = cp;
					contacts->separation = sqrtf(distSqr) - radius;
					return 1;
				}
			}
		}
		//contacts->tnormal = h02.UnitVec();
	}

	if (inside21 > 0.f && inside02 > 0.f && inside10 > 0.f) {
		// closest point is inside triangle
		float u = 1.f / (inside02 + inside10 + inside21);
		float w0 = inside21 * u;
		float w1 = inside02 * u;
		float w2 = 1.f - w0 - w1; // inside10 * u;
		contacts->position = h0 * w0 + h1 * w1 + h2 * w2;
		contacts->normal = triNormal;
		contacts->separation = centerLevel;
		return 1;
	}
	return 0;
}

}
