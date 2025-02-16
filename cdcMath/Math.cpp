#include "Math.h"
#include "VectorInlines.h"
#include <cstring>
#include <cmath>

namespace cdc {

void Vector::SafeNormalize3() {
    // HACK
    float d = x*x + y*y + z*z;
    if (d > 0.0f) {
        d = sqrt(d);
        d = 1.0f/d;
        x *= d;
        y *= d;
        z *= d;
    }
}

void Vector::Normalize3() {
    // HACK
    float d = x*x + y*y + z*z;
    d = sqrt(d);
    d = 1.0f/d;
    *this *= d;
}

Matrix operator*(MatrixArg matA, float valB) { // Matrix.cpp:58
    return {{
        { matA.m[0][0] * valB, matA.m[0][1] * valB, matA.m[0][2] * valB, matA.m[0][3] * valB },
        { matA.m[1][0] * valB, matA.m[1][1] * valB, matA.m[1][2] * valB, matA.m[1][3] * valB },
        { matA.m[2][0] * valB, matA.m[2][1] * valB, matA.m[2][2] * valB, matA.m[2][3] * valB },
        { matA.m[3][0] * valB, matA.m[3][1] * valB, matA.m[3][2] * valB, matA.m[3][3] * valB }
    }};
}

Matrix operator*(float valA, MatrixArg matB) { // Matrix.cpp:68
    return matB * valA;
}

Matrix Mul3x3(MatrixArg matA, MatrixArg matB) { // Matrix.cpp:112
    Matrix const& m1 = matA;
    Matrix const& m2 = matB;
    return {
        m1.m[0][0] * m2.m[0][0] + m1.m[1][0] * m2.m[0][1] + m1.m[2][0] * m2.m[0][2],
        m1.m[0][1] * m2.m[0][0] + m1.m[1][1] * m2.m[0][1] + m1.m[2][1] * m2.m[0][2],
        m1.m[0][2] * m2.m[0][0] + m1.m[1][2] * m2.m[0][1] + m1.m[2][2] * m2.m[0][2],
        0.f,

        m1.m[0][0] * m2.m[1][0] + m1.m[1][0] * m2.m[1][1] + m1.m[2][0] * m2.m[1][2],
        m1.m[0][1] * m2.m[1][0] + m1.m[1][1] * m2.m[1][1] + m1.m[2][1] * m2.m[1][2],
        m1.m[0][2] * m2.m[1][0] + m1.m[1][2] * m2.m[1][1] + m1.m[2][2] * m2.m[1][2],
        0.f,

        m1.m[0][0] * m2.m[2][0] + m1.m[1][0] * m2.m[2][1] + m1.m[2][0] * m2.m[2][2],
        m1.m[0][1] * m2.m[2][0] + m1.m[1][1] * m2.m[2][1] + m1.m[2][1] * m2.m[2][2],
        m1.m[0][2] * m2.m[2][0] + m1.m[1][2] * m2.m[2][1] + m1.m[2][2] * m2.m[2][2],
        0.f,

        0.f,
        0.f,
        0.f,
        1.f, // huh
    };
}

void Matrix::Build(QuatArg q) { // Matrix.cpp:169
    // TODO
}

void Matrix::Build_XYZOrder(float *angles) { // Matrix.cpp:283
    *this = identity4x4;
    RotZ(sin(angles[2]), cos(angles[2]));
    RotY(sin(angles[1]), cos(angles[1]));
    RotX(sin(angles[0]), cos(angles[0]));
}

void Matrix::RotX(float sina, float cosa) { // Matrix.cpp:312
    float t0, t1;
    for (int i=0; i<4; i++) {
        t0 = m[i][1];
        t1 = m[i][2];
        m[i][1] = t0 * cosa - t1 * sina;
        m[i][2] = t0 * sina + t1 * cosa;
    }
}

void Matrix::RotY(float sina, float cosa) { // Matrix.cpp:333
    float t0, t1;
    for (int i=0; i<4; i++) {
        t0 = m[i][2];
        t1 = m[i][0];
        m[i][2] = t0 * cosa - t1 * sina;
        m[i][0] = t0 * sina + t1 * cosa;
    }
}

void Matrix::RotZ(float sina, float cosa) { // Matrix.cpp:353
    float t0, t1;
    for (int i=0; i<4; i++) {
        t0 = m[i][0];
        t1 = m[i][1];
        m[i][0] = t0 * cosa - t1 * sina;
        m[i][1] = t0 * sina + t1 * cosa;
    }
}

void OrthonormalInverse3x4(Matrix *result, Matrix& m) { // Matrix.cpp:373
    result->m[3][0] = -(m.m[0][0] * m.m[3][0] + m.m[0][1] * m.m[3][1] + m.m[0][2] * m.m[3][2]);
    result->m[3][1] = -(m.m[1][0] * m.m[3][0] + m.m[1][1] * m.m[3][1] + m.m[1][2] * m.m[3][2]);
    result->m[3][2] = -(m.m[2][0] * m.m[3][0] + m.m[2][1] * m.m[3][1] + m.m[2][2] * m.m[3][2]);

    for (int i=0; i<3; i++) {
        result->m[i][0] = m.m[0][i];
        result->m[i][1] = m.m[1][i];
        result->m[i][2] = m.m[2][i];
    }

    result->m[0][3] = 0.0f;
    result->m[1][3] = 0.0f;
    result->m[2][3] = 0.0f;
    result->m[3][3] = 1.0f;
}

// column-major multiplication
Matrix operator*(const Matrix& m1, const Matrix& m2)
{
    return
    {
        m1.m[0][0] * m2.m[0][0] + m1.m[1][0] * m2.m[0][1] + m1.m[2][0] * m2.m[0][2] + m1.m[3][0] * m2.m[0][3],
        m1.m[0][1] * m2.m[0][0] + m1.m[1][1] * m2.m[0][1] + m1.m[2][1] * m2.m[0][2] + m1.m[3][1] * m2.m[0][3],
        m1.m[0][2] * m2.m[0][0] + m1.m[1][2] * m2.m[0][1] + m1.m[2][2] * m2.m[0][2] + m1.m[3][2] * m2.m[0][3],
        m1.m[0][3] * m2.m[0][0] + m1.m[1][3] * m2.m[0][1] + m1.m[2][3] * m2.m[0][2] + m1.m[3][3] * m2.m[0][3],

        m1.m[0][0] * m2.m[1][0] + m1.m[1][0] * m2.m[1][1] + m1.m[2][0] * m2.m[1][2] + m1.m[3][0] * m2.m[1][3],
        m1.m[0][1] * m2.m[1][0] + m1.m[1][1] * m2.m[1][1] + m1.m[2][1] * m2.m[1][2] + m1.m[3][1] * m2.m[1][3],
        m1.m[0][2] * m2.m[1][0] + m1.m[1][2] * m2.m[1][1] + m1.m[2][2] * m2.m[1][2] + m1.m[3][2] * m2.m[1][3],
        m1.m[0][3] * m2.m[1][0] + m1.m[1][3] * m2.m[1][1] + m1.m[2][3] * m2.m[1][2] + m1.m[3][3] * m2.m[1][3],

        m1.m[0][0] * m2.m[2][0] + m1.m[1][0] * m2.m[2][1] + m1.m[2][0] * m2.m[2][2] + m1.m[3][0] * m2.m[2][3],
        m1.m[0][1] * m2.m[2][0] + m1.m[1][1] * m2.m[2][1] + m1.m[2][1] * m2.m[2][2] + m1.m[3][1] * m2.m[2][3],
        m1.m[0][2] * m2.m[2][0] + m1.m[1][2] * m2.m[2][1] + m1.m[2][2] * m2.m[2][2] + m1.m[3][2] * m2.m[2][3],
        m1.m[0][3] * m2.m[2][0] + m1.m[1][3] * m2.m[2][1] + m1.m[2][3] * m2.m[2][2] + m1.m[3][3] * m2.m[2][3],

        m1.m[0][0] * m2.m[3][0] + m1.m[1][0] * m2.m[3][1] + m1.m[2][0] * m2.m[3][2] + m1.m[3][0] * m2.m[3][3],
        m1.m[0][1] * m2.m[3][0] + m1.m[1][1] * m2.m[3][1] + m1.m[2][1] * m2.m[3][2] + m1.m[3][1] * m2.m[3][3],
        m1.m[0][2] * m2.m[3][0] + m1.m[1][2] * m2.m[3][1] + m1.m[2][2] * m2.m[3][2] + m1.m[3][2] * m2.m[3][3],
        m1.m[0][3] * m2.m[3][0] + m1.m[1][3] * m2.m[3][1] + m1.m[2][3] * m2.m[3][2] + m1.m[3][3] * m2.m[3][3],
    };
}

bool operator==(const Matrix& m1, const Matrix& m2) {
    // original game does proper float comparisons (0.0f == -0.0f)
    return memcmp((void*)&m1, (void*)&m2, sizeof(Matrix)) == 0;
}

Vector& operator+=(Vector& a, VectorArg b) {
    a.x += b.x;
    a.y += b.y;
    a.z += b.z;
    a.w += b.w;
    return a;
}

Vector operator+(VectorArg a, VectorArg b) {
    return {a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w};
}

Vector operator-(VectorArg a, VectorArg b) {
    return {a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w};
}

Vector operator*(VectorArg a, float b) {
    return {a.x * b, a.y * b, a.z * b, a.w * b};
}

Vector operator-(VectorArg a) {
    return {-a.x, -a.y, -a.z, -a.w};
}

Matrix transpose(Matrix m) {
    return {{
        {m.m[0][0], m.m[1][0], m.m[2][0], m.m[3][0]},
        {m.m[0][1], m.m[1][1], m.m[2][1], m.m[3][1]},
        {m.m[0][2], m.m[1][2], m.m[2][2], m.m[3][2]},
        {m.m[0][3], m.m[1][3], m.m[2][3], m.m[3][3]}
    }};
}

Matrix identity4x4 = {{
    {1.0f, 0.0f, 0.0f, 0.0f},
    {0.0f, 1.0f, 0.0f, 0.0f},
    {0.0f, 0.0f, 1.0f, 0.0f},
    {0.0f, 0.0f, 0.0f, 1.0f}
}};

} // namespace cdc
