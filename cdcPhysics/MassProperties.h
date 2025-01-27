#pragma once
#include "cdcMath/Math.h"

namespace cdc {

struct MassProperties {
	float mass;
	Vector3 center;
	Matrix I;

	void SetZero();
	void Set(float mass, Vector3Arg center, MatrixArg I);
	void Set(float mass, MatrixArg I);
	void SetBox(float mass, Vector3Arg);
	void SetCapsule(float mass, float r, float L);
	void SetSphere(float mass, float r);
	void Translate(Vector3Arg);
	void Rotate(MatrixArg R);
	void Add(MassProperties const& other);
	bool CheckValidity() const;
};

}
