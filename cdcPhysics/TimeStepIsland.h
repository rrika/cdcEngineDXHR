#pragma once

namespace cdc {

class TimeStepIsland {
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
