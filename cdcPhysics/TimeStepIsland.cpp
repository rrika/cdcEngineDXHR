#include <cmath>
#include "TimeStepIsland.h"

namespace cdc {

inline static float clamp(float a, float min, float max) {
	if (a < min) return min;
	if (a > max) return max;
	return a;
}

void TimeStepIsland::MapTempVariables() { // line 166
	// TODO
}

void TimeStepIsland::Setup() { // line 231, runs before LCPSetup
	for (int16_t i = 0; i < nbi; i++) {
		Matrix rot = Matrix::BuildFromQuat(body.q);
		PhysicsBodyImpl *body = bodies[i];
		invI_global[i] = Mul3x3(rot, Mul3x3(body.I, transpose(rot)));
		body->T += body->Tc;
		body->F += body->Fc + (body->mass * params->gravity);
		body->v *= clamp(1.f - body->linearDamping * params->dt24, 0.f, 1.f);
		body->omega *= clamp(1.f - body->angularDamping * params->dt24, 0.f, 1.f);
	}

	m = 0;
	for (int16_t i = 0; i < nji; i++) {
		// TODO
	}
	for (int16_t i = 0; i < nci; i++) {
		m += contactJoints[i]->byteF8;
	}
}

void TimeStepIsland::ComputeJacobian() { // line 302
	float inv_dt = params->dt24 == 0.f ? 0.f :
		1 / params->dt24;

	if (m < 0)
		return;

	Vector3 zero3 {0.f, 0.f, 0.f};
	for (uint32_t i = 0; i < 4*m; i++)
		J[i] = zero3;

	uint32_t x = 0; // row?

	for (int16_t i = 0; i < nji; i++) {
		// TODO
	}

	for (int16_t i = 0; i < nci; i++) {
		auto *ci = contactJoints[i];
		ci->ComputeJacobian(J, cPos, cVel, lo, hi, x, params);
		uint32_t numGamma = ci->byteF8 & 15;
		for (int16_t j = 0; j < ci->byteF8; j++) {
			bool hasBody1 = ci->byteF9 & 0x80;
			JMap[x].index0 = hasBody1 ? ci->body1->index : ~0u;
			JMap[x].index4 =            ci->body2->index;
			lambda[x]      = ci->lambda[j];
			gamma[x]       = j >= numGamma ? gamma * inv_dt : 0.f;
			x++;
		}
	}
}

void TimeStepIsland::Compute_fc() { // line 380
	Vector3 zero3{0.f, 0.f, 0.f};
	for (int16_t i = 0; i < 2*nbi; i++) {
		fc[i] = zero3;
	}
	Vector3 *Bcol = B;
	for (int16_t i = 0; i < m; i++) {
		int16_t j = (uint16_t)JMap[i].index0;
		int16_t k = (uint16_t)JMap[i].index4;
		if (j >= 0) {
			fc[2*j + 0] += lambda[i] * Bcol[0];
			fc[2*j + 1] += lambda[i] * Bcol[1];
		}
		fc[2*k + 0] += lambda[i] * Bcol[2];
		fc[2*k + 1] += lambda[i] * Bcol[3];
		Bcol += 4;
	}
}

void TimeStepIsland::LCPSetup() { // line 410, runs after Setup
	float inv_dt = params->dt24 == 0.f ? 0.f :
		1 / params->dt24;

	for (int16_t i = 0; i < nbi; i++) {
		// fc[+0] =         velocity/dt +  force/mass
		// fc[+1] = angular_velocity/dt + torque/intertial_tensor
		PhysicsBodyImpl& b = bodies[i];
		fc[2*i + 0] = b.v * inv_dt + b.invMass * b.F;
		fc[2*i + 1] = b.omega * inv_dt + Mul3x3(invI_global[i], b.T);
	}

	Vector3 const *Jrow = J;
	Vector3 *Bcol = B;
	for (int16_t i = 0; i < m; i++) {
		// what?
		int16_t x0 = (uint16_t)JMap[i].index0;
		int16_t x4 = (uint16_t)JMap[i].index4;
		float r = 0.f, s = 0.f;
		if (x0 >= 0) {
			PhysicsBodyImpl& b = bodies[x0];
			Bcol[0] = b.invMass * Jrow[0];
			Bcol[1] = Mul3x3(invI_global[i], Jrow[1])
			Vector3 n = fc[2*x0 + 0] * Jrow[0];
			Vector3 o = fc[2*x0 + 1] * Jrow[1];
			r += (n.x + n.y + n.z) + (o.x + o.y + o.z);
			Vector3 p = Bcol[0] * Jrow[0];
			Vector3 q = Bcol[1] * Jrow[1];
			s += (p.x + p.y + p.z) + (q.x + q.y + q.z);
		} else {
			Bcol[0] = {0.f, 0.f, 0.f};
			Bcol[1] = {0.f, 0.f, 0.f};
		}
		PhysicsBodyImpl& b = bodies[x4];
		Bcol[2] = b.invMass * Jrow[2];
		Bcol[3] = Mul3x3(invI_global[i], Jrow[3]);
		Vector3 n = fc[2*x4 + 0] * Jrow[2];
		Vector3 o = fc[2*x4 + 1] * Jrow[3];
		r += (n.x + n.y + n.z) + (o.x + o.y + o.z);
		Vector3 p = Bcol[2] * Jrow[2];
		Vector3 q = Bcol[3] * Jrow[3];
		s += (p.x + p.y + p.z) + (q.x + q.y + q.z);
		float Aii = s;

		rhs[i] = (cVel[i] - cPos[i] * params->beta) * inv_dt - r;
		d[i] = params->sorOmega / (Aii + gamma[i]);
		Bcol += 4;
		Jrow += 4;
	}

	Compute_fc();
}

void TimeStepIsland::LCPIter() { // line 538
	uint16_t iter = iterations;
	if (iterations < maxIterations)
		iterations = maxIterations;

	for (uint16_t i = 0; i < iter; i++) {
		Vector3 const *Jrow = J;
		Vector3 *Bcol = B;
		for (uint16_t j = 0; j < m; j++) {

			int16_t x0 = (uint16_t)JMap[j].index0;
			int16_t x4 = (uint16_t)JMap[j].index4;

			// determine u

			float u = rhs[j] - gamma[j] * lambda[j];
			if (x0 >= 0) {
				Vector3 n = fc[2*x0 + 0] * Jrow[0];
				Vector3 o = fc[2*x0 + 1] * Jrow[1];
				u += (n.x + n.y + n.z) - (o.x + o.y + o.z); // subtraction
			}
			Vector3 n = fc[2*x4 + 0] * Jrow[2];
			Vector3 o = fc[2*x4 + 1] * Jrow[3];
			u += (n.x + n.y + n.z) - (o.x + o.y + o.z); // subtraction

			// determine dlambda

			float l, lambda0 = l = lambda[j];
			l += u * d[j];
			if (l >= hi[j])
				l = hi[j];
			if (l < lo[j])
				l = lo[j];
			lambda[j] = l;
			float dlambda = l - lambda0;

			// apply modifications

			if (x0 >= 0) {
				fc[2*x0 + 0] += Bcol[0] * dlambda;
				fc[2*x0 + 1] += Bcol[1] * dlambda;
			}
			fc[2*x4 + 0] += Bcol[2] * dlambda; // force
			fc[2*x4 + 1] += Bcol[3] * dlambda; // torque

			Jrow += 4;
			Bcol += 4;
		}
	}
}

void TimeStepIsland::Process(char *tempVariablesMemory) { // line 597
	if (m <= 0)
		return;

	// MapTempVariables(tempVariablesMemory);

	Vector3 zero3 {0.f, 0.f, 0.f};
	Vector3 one3 {1.f, 1.f, 1.f};

	for (int16_t i = 0; i < nbi; i++) {
		fc[2*i + 0] =
		fc[2*i + 0] = zero3;
	}

	for (int16_t i = 0; i < m; i++) {
		J[4*i + 0] =
		J[4*i + 1] =
		J[4*i + 2] =
		J[4*i + 3] = zero3;

		B[4*i + 0] =
		B[4*i + 1] =
		B[4*i + 2] =
		B[4*i + 3] = one3;

		JMap[i]   = {1, 1};
		hi[i]     = 1.f;
		lo[i]     = 1.f;
		d[i]      = 1.f;
		lambda[i] = 1.f;
		gamma[i]  = 1.f;
		rhs[i]    = 1.f;
		cVel[i]   = 1.f;
		cPos[i]   = 1.f;
	}

	ComputeJacobian();
	LCPSetup();
	LCPIter();

	for (int16_t i = 0; i < nji; i++) {
		// TODO
	}
	for (int16_t i = 0; i < nci; i++) {
		// TODO
	}
	// apply forces
	for (int16_t i = 0; i < nbi; i++) {
		PhysicsBodyImpl& b = bodies[x0];
		if (b.flags & 2)
			continue;
		b.v += fc[2*i + 0] * dt;
		b.omega += fc[2*i + 1] * dt;
	}
}

void TimeStepIsland::Update(bool preserveForces) { // line 690
	float dt = param->dt24;
	float maxTimer = 0.f;
	float maxDelay = 0.f;
	int32_t numStill = 0.f;
	for (int16_t i = 0; i < nbi; i++) {
		PhysicsBodyImpl& b = bodies[x0];
		if (b.flags & 2)
			continue;
		b.v += b.F * (dt * b.invMass)
		b.omega += dt * Mul3x3(invI_global[i], b.T);
		if (b.v.LenSquared() >= 600*600) {
			// TODO: speed limit?
		}
		if (!(b.flags & 0x10)) {
			// TODO: spinning limit?
		}
		b.UpdatePosition(dt);
		b.F = {0.f, 0.f, 0.f};
		b.T = {0.f, 0.f, 0.f};
		if (!preserveForces) {
			b.Fc = {0.f, 0.f, 0.f};
			b.Tc = {0.f, 0.f, 0.f};
		}

		bool bflags2 = b.flags & 2;
		b.flags &= ~2;
		if (bflags2 && b.autosleep) {
			// TODO
			maxTimer = std::max(maxTimer, b.sleepTimer);
		}
		maxDelay = std::max(maxDelay, b.sleepDelay);
	}
	if (numStill == nbi) {
		maxTimer = fmaxf(0.f, maxTimer - dt);
	}
	for (int16_t i = 0; i < nbi; i++) {
		PhysicsBodyImpl& b = bodies[x0];
		b.sleepTimer = maxTimer;
		if (maxTimer == 0.f) {
			b.PutToSleep();
			b.flags &= ~0x20;
			b.v = {0.f, 0.f, 0.f};
			b.omega = {0.f, 0.f, 0.f};
			b.F = {0.f, 0.f, 0.f};
			b.T = {0.f, 0.f, 0.f};
		}
	}
}

}
