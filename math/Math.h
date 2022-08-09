#pragma once

struct float4x4 { float m[4][4]; };
float4x4 operator*(const float4x4& m1, const float4x4& m2);
bool operator==(const float4x4& m1, const float4x4& m2);
inline bool operator!=(const float4x4& m1, const float4x4& m2) { return !(m1 == m2); }
float4x4 transpose(float4x4);
extern float4x4 identity4x4;

namespace cdc {

class Vector {
public:
	union {
		float vec128[4];
		struct {
			float x;
			float y;
			float z;
			float w;
		};
	};
};

class Quat : public Vector {};
class Vector2 : public Vector {};
class Vector3 : public Vector {};
class Vector4 : public Vector {};

using Matrix = float4x4;

}
