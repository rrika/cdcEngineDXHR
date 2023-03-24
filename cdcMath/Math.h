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

	void SafeNormalize3();
};

class Quat : public Vector {};
class Scalar : public Vector {};
class Vector2 : public Vector {};
class Vector3 : public Vector {};
class Vector4 : public Vector {};
using QuatArg = Quat const&;
using ScalarArg = Scalar const&;
using VectorArg = Vector const&;
using Vector2Arg = Vector2 const&;
using Vector3Arg = Vector3 const&;
using Vector4Arg = Vector4 const&;

struct Matrix { float m[4][4]; };
using MatrixArg = Matrix const&;

inline float Abs(float A) { return A < 0 ? -A : A; } // line 2931

void OrthonormalInverse3x4(Matrix *result, Matrix& m);

Matrix operator*(const Matrix& m1, const Matrix& m2);
bool operator==(const Matrix& m1, const Matrix& m2);
inline bool operator!=(const Matrix& m1, const Matrix& m2) { return !(m1 == m2); }
Matrix transpose(Matrix);
extern Matrix identity4x4;

Vector& operator+=(Vector& a, VectorArg b);
Vector operator+(VectorArg a, VectorArg b);
Vector operator*(VectorArg a, float b);

}
