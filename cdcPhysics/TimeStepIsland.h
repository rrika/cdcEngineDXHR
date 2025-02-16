#pragma once
#include <cstdint>

namespace cdc {

class ContactJoint;
class PhysicsBodyImpl;
class Vector3;
struct Matrix;
struct TimeStepIslandParams;

struct JMapEntry {
	uint32_t index0;
	uint32_t index4;
};

class TimeStepIsland {
	TimeStepIslandParams *params; // 0
	uint32_t dword4;
	uint32_t dword8;
	Matrix *invI_global; // C
	int32_t nbi; // 10
	int32_t nji; // 14
	int32_t nci; // 18
	uint32_t m; // 1C
	uint16_t iterations; // 20
	uint16_t minIterations; // 22
	// uint32_t dword24;
	// uint32_t dword28;
	// uint32_t dword2C;
	PhysicsBodyImpl **bodies; // 30
	void *joints; // 34
	ContactJoint **contactJoints; // 38
	Vector3 *fc; // 3C
	Vector3 *J; // 40
	Vector3 *B; // 44
	JMapEntry *JMap; // 48
	float *cPos; // 4C
	float *cVel; // 50
	float *rhs; // 54
	float *gamma; // 58
	float *lambda; // 5C
	float *d; // 60
	float *lo; // 64
	float *hi; // 68
	// uint32_t dword6C;
public:
	void MapTempVariables(); // 166
	void Setup(); // 231
	void ComputeJacobian(); // 302
	void Compute_fc(); // 380
	void LCPSetup(); // 410
	void LCPIter(); // 538
	void Process(char *tempVariablesMemory); // 597
	void Update(bool preserveForces); // 690
};

}
