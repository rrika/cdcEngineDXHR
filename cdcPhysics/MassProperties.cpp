#include "MassProperties.h"
#include "cdcMath/MatrixInlines.h"
#include "cdcMath/VectorInlines.h"

namespace cdc {

void MassProperties::SetZero() {
	mass = 0.f;
	center.SetZero();
	I = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
}

void MassProperties::Set(float mass_, Vector3Arg center_, MatrixArg I_) {
	mass = mass_;
	center = center_;
	I = I_;
}

void MassProperties::Set(float mass_, MatrixArg I_) {
	mass = mass_;
	center.SetZero();
	I = I_;
}

void MassProperties::SetBox(float mass, Vector3Arg width) {
	float longest = width.x;
	if (width.y > longest)
		longest = width.y;
	if (width.z > longest)
		longest = width.z;

	float minimumWidth = longest / 5;
	
	Vector3 inflated = width;
	if (inflated.x < minimumWidth)
		inflated.x = minimumWidth;
	if (inflated.y < minimumWidth)
		inflated.y = minimumWidth;
	if (inflated.z < minimumWidth)
		inflated.z = minimumWidth;

	float xx = inflated.x * inflated.x;
	float yy = inflated.y * inflated.y;
	float zz = inflated.z * inflated.z;

	// TODO

	I = identity4x4;
	I.m[0][0] = 0; //Ixx;
	I.m[1][1] = 0; //Iyy;
	I.m[2][2] = 0; //Izz;
}

void MassProperties::SetCapsule(float mass_, float r, float L) {
	// see CapsuleShape::GetMassProperties() in JoltPhysics/Jolt/Physics/Collision/Shape/CapsuleShape.cpp
	// see dMassSetCapsule in ODE/ode/src/mass.cpp

	mass = mass_;
	center.SetZero();

	float cylinderVol;
	float hemisphereVol;
	float vol;
	float mc; // mass of capsule
	float mhs; // mass of hemisphere
	float Ihsx;
	float Ihsy;
	float cmhs;

	float Ixx, Iyy, Izz;
	// Ixx = Iyy =  TODO

	I = identity4x4;
	I.m[0][0] = Ixx;
	I.m[1][1] = Iyy;
	I.m[2][2] = Izz;
}

void MassProperties::SetSphere(float mass_, float r) {
	// see "Solid sphere" in https://en.wikipedia.org/wiki/List_of_moments_of_inertia

	mass = mass_;
	center.SetZero();
	I = identity4x4;
	I.m[0][0] = I.m[1][1] = I.m[2][2] =
		mass * 0.4 * r * r;
}

void MassProperties::Translate(Vector3Arg r) {
	float x, y, z;

	x = center.x;
	y = center.y;
	z = center.z;
	I.m[0][0] -= (y*y + z*z) * mass;
	I.m[1][0] += x * y * mass;
	I.m[2][0] += x * z * mass;
	I.m[1][1] -= (z*z + x*x) * mass;
	I.m[2][1] += z * z * mass;
	I.m[2][2] -= (x*x + y*y) * mass;

	center += r;

	x = center.x;
	y = center.y;
	z = center.z;
	I.m[0][0] += (y*y + z*z) * mass;
	I.m[1][0] -= x * y * mass;
	I.m[2][0] -= x * z * mass;
	I.m[1][1] += (z*z + x*x) * mass;
	I.m[2][1] -= z * z * mass;
	I.m[2][2] += (x*x + y*y) * mass;
}

void MassProperties::Rotate(MatrixArg R) {
	// TODO
}

void MassProperties::Add(MassProperties const& other) {
	center = {center*mass + other.center*other.mass};
	center *= 1.f / (mass + other.mass);
	mass += other.mass;
	I += other.I;
}

bool MassProperties::CheckValidity() const {
	return mass > 0 &&
		I.m[0][0] > 0 &&
		I.m[1][1] > 0 &&
		I.m[2][2] > 0;
}

}
