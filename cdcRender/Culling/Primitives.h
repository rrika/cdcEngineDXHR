#pragma once
#include <vector>
#include "cdcMath/Math.h"
#include "BasicPrimitives.h"

namespace cdc {

struct RenderViewport;

// HACK
template <typename T> using CullingArray = std::vector<T>;

struct CullingFrustum { // 160
	using VectorArray = CullingArray<Vector3>;
	using PlaneArray = CullingArray<CullingPlane>;

	struct Perspective { // 169
		float fov;
		float aspect;
		float nearz;
		float farz;
	};

	struct Orthographic { // 177
		float width;
		float height;
		float nearz;
		float farz;
	};

	void Set(Perspective& data);
	void Set(Orthographic& data);
	void Set(RenderViewport& viewport);
	void Transform(Matrix& m);
	void Discard();

	Vector3 m_eye;
	bool m_perspective;
	PlaneArray m_planes;
	VectorArray m_vertices;
};

bool Intersects(CullingSphere const& a, CullingFrustum const& b); // 2004
bool Intersects(CullingBox const& a, CullingFrustum const& b); // 2021

}
