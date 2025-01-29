#include "TimeStepIsland.h"

namespace cdc {

void TimeStepIsland::MapTempVariables() { // line 166
	// TODO
}

void TimeStepIsland::Setup() { // line 231
	// TODO
}

void TimeStepIsland::ComputeJacobian() { // line 302
	// TODO
}

void TimeStepIsland::Compute_fc() { // line 380
	// TODO
}

void TimeStepIsland::LCPSetup() { // line 410
	// TODO
	Compute_fc();
}

void TimeStepIsland::LCPIter() { // line 538
	// TODO
}

void TimeStepIsland::Process(char *tempVariablesMemory) { // line 597
	// TODO
	ComputeJacobian();
	LCPSetup();
	LCPIter();
}

void TimeStepIsland::Update(bool preserveForces) { // line 690
	// TODO
	// body->UpdatePosition();
}

}
