#pragma once

namespace cdc {

class Vector3;
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

	void SetZero();
	void SafeNormalize3();
	void Normalize3();
	bool IsZero2() const;
	bool IsZero3() const;
	bool IsZero4() const;
	inline Vector& operator*=(float valB);
};

class Quat : public Vector {};
class Euler : public Vector {};
class Scalar : public Vector {};
class Vector2 : public Vector {};
class Vector3 : public Vector {
public:
	inline float LenSquared() const;
	Vector3 UnitVec() {
		Vector3 copy = *this;
		copy.Normalize3();
		return copy;
	}
};
class Vector4 : public Vector {};
using QuatArg = Quat const&;
using EulerArg = Euler const&;
using ScalarArg = Scalar const&;
using VectorArg = Vector const&;
using Vector2Arg = Vector2 const&;
using Vector3Arg = Vector3 const&;
using Vector4Arg = Vector4 const&;

struct Matrix {
	float m[4][4];
	void Build_XYZOrder(float *angles);
	void RotZ(float sina, float cosa);
	void RotY(float sina, float cosa);
	void RotX(float sina, float cosa);
	inline Matrix& operator*=(float t);
	inline Matrix& operator+=(Matrix const& b);
};

using MatrixArg = Matrix const&;

inline float Abs(float A) { return A < 0 ? -A : A; } // line 2931

void OrthonormalInverse3x4(Matrix *result, Matrix& m);

Matrix operator*(const Matrix& m1, const Matrix& m2);
Matrix operator*(MatrixArg matA, float valB);
Matrix operator*(float valA, MatrixArg matB);
bool operator==(const Matrix& m1, const Matrix& m2);
inline bool operator!=(const Matrix& m1, const Matrix& m2) { return !(m1 == m2); }
Matrix transpose(Matrix);
extern Matrix identity4x4;

Vector& operator+=(Vector& a, VectorArg b);
Vector operator+(VectorArg a, VectorArg b);
Vector operator-(VectorArg a, VectorArg b);
Vector operator*(VectorArg a, float b);
Vector operator-(VectorArg a);

}
