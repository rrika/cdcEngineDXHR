#pragma once

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
using VectorArg = Vector const&;

struct Matrix { float m[4][4]; };

}

cdc::Matrix operator*(const cdc::Matrix& m1, const cdc::Matrix& m2);
bool operator==(const cdc::Matrix& m1, const cdc::Matrix& m2);
inline bool operator!=(const cdc::Matrix& m1, const cdc::Matrix& m2) { return !(m1 == m2); }
cdc::Matrix transpose(cdc::Matrix);
extern cdc::Matrix identity4x4;

cdc::Vector& operator+=(cdc::Vector& a, cdc::VectorArg b);
cdc::Vector operator+(cdc::VectorArg a, cdc::VectorArg b);
cdc::Vector operator*(cdc::VectorArg a, float b);
