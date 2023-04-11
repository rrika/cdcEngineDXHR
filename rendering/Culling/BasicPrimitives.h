#pragma once
#include "cdcMath/Math.h"

namespace cdc {

enum CullingVolumeType { // line 27
	kVolumeSphere = 0,
	kVolumeBox = 5,
	kVolumeNothing = 9,
	kVolumeEverything = 10
};

struct CullingMinMax : Vector2 {}; // line 57

struct CullingPlane { // line 84
	inline void Set(Vector3Arg normal, Vector3Arg point);
	inline void Set(Vector3Arg v1, Vector3Arg v2, Vector3Arg v3);
	inline void Transform(Matrix const& m);

	Vector4 m_planeEq;
};

struct CullingSphere { // line 122
	inline void Transform(Matrix const& m);

	Vector4 m_sphereEq;
};

struct CullingBox { // line 296
	inline void SetFromCenterAndExtents(Vector3Arg center, Vector3Arg extents);
	inline void SetFromMinMax(Vector3Arg min, Vector3Arg max);
	inline CullingMinMax GetProjectedMinMax(Vector3Arg e) const;
	inline void Translate(Vector3Arg t);
	inline void Transform(Matrix const& m);

	Vector4 m_exX;
	Vector4 m_exY;
	Vector4 m_exZ;
	Vector4 m_center;
};

struct BasicCullingVolume { // line 471
	void Transform(Matrix const& m);

	union DataBuffer { // line 526
		CullingSphere sphere;
		CullingBox box;
	};

	DataBuffer m_data;
	CullingVolumeType m_type = kVolumeEverything;
};

}
