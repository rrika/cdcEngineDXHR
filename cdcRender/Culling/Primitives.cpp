#include "cdcMath/MatrixInlines.h"
#include "Primitives.h"
#include "BasicPrimitives_inlines.h"
#include "cdcRender/CommonScene.h"

namespace cdc {

void CullingFrustum::Set(CullingFrustum::Perspective& data) { // line 66
	VectorArray& v = m_vertices;
	PlaneArray& p = m_planes;

	float x, y, z, t;
	v.resize(8);
	t = tan(data.fov * 0.5);

	z = data.nearz;
	y = z * t;
	x = y * data.aspect;
	v[0] = {-x, -y, z, 0.0f};
	v[1] = { x, -y, z, 0.0f};
	v[2] = { x,  y, z, 0.0f};
	v[3] = {-x,  y, z, 0.0f};

	z = data.farz;
	y = z * t;
	x = y * data.aspect;
	v[4] = {-x, -y, z, 0.0f};
	v[5] = { x, -y, z, 0.0f};
	v[6] = { x,  y, z, 0.0f};
	v[7] = {-x,  y, z, 0.0f};

	p.resize(6);
	p[0].Set(v[0], v[2], v[1]);
	p[1].Set(v[6], v[4], v[5]);
	p[2].Set(v[0], v[7], v[3]);
	p[3].Set(v[1], v[4], v[0]);
	p[4].Set(v[2], v[5], v[1]);
	p[5].Set(v[3], v[6], v[2]);
}

void CullingFrustum::Set(CullingFrustum::Orthographic& data) { // line 137
	// TODO
}

void CullingFrustum::Set(RenderViewport& viewport) { // line 137
	if (viewport.fov == 0.0f) {
		Orthographic o {
			viewport.width, // 28
			viewport.height, // 2C
			viewport.nearz, // 18
			viewport.farz // 1C
		};
		Set(o);
		Transform(viewport.viewMatrix);
	} else {
		Perspective p {
			viewport.fov,
			viewport.aspect,
			viewport.nearz,
			viewport.farz
		};
		Set(p);
		Transform(viewport.viewMatrix);
	}
}

void CullingFrustum::Transform(Matrix& m) { // 365
	for (auto& v : m_vertices)
		v = m * v;
	for (auto& p : m_planes)
		p.Transform(m);

	// TODO: flip planes if handedness changes
	// for (auto& p : m_planes)
	// 	p.m_planeEq = Vector4{Vector{p.m_planeEq} * -1.0f};
}

void CullingFrustum::Discard() { // line 416
	m_planes.clear();
	m_vertices.clear();
}

bool Intersects(CullingSphere const& a, CullingFrustum const& b) { // line 2004
	// the frustrum planes point outward
	for (auto& plane : b.m_planes)
		if (InFront(plane, a))
			return false;

	return true;
}

bool Intersects(CullingBox const& a, CullingFrustum const& b) { // line 2021
	// the frustrum planes point outward
	for (auto& plane : b.m_planes) {
		Vector3 axis{plane.m_planeEq};
		auto minmax = a.GetProjectedMinMax(axis);
		if (minmax.x > plane.m_planeEq.w)
			return false;
	}

	return true;
}

}
