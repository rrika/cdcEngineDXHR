#include <cstring>
#include "MultibodyClient.h"
#include "cdcMath/Math.h"
#include "cdcMath/MatrixInlines.h"
#include "cdcPhysics/MassProperties.h"
#include "cdcPhysics/PhysicsBody.h" // SetName etc. are inlines
#include "cdcSys/Trace.h"
#include "cdcWorld/Instance.h"
#include "cdcWorld/Object.h"

using namespace cdc;

MultibodyClient *gMultibodySystem = nullptr;

void MULTIBODY_PrintWarning(const char *fmt, ...) { // line 646
	// TODO
}

bool MultibodyClient::CreateBodyFromModel(
	Instance *instance, int32_t modelNum,
	dtp::rigidbody const *prop, Matrix *pMatCenterOfMass, float massOverride
) {
	auto *hmodel = &instance->collideComponent.GetHModelList()[modelNum];
	if (!hmodel || hmodel->numHPrims <= 0)
		return false;

	// TODO: loop over prims

	auto *body = CreateBody();
	body->SetClientData((void*)instance);
	if (strlen(instance->object->name) >= 0x80)
		body->SetName("NAME TOO LONG");
	else {
		char buffer[256];
		// sprintf(buffer, "%s%02d", instance->object->name, instance->introNum);
		body->SetName(instance->object->name);
	}
	MassProperties massProp;
	CalcMassPropertiesFromHModel(hmodel, &massProp);
	if (massOverride > 0.f) {
		massProp.I *= massOverride / massProp.mass;
		massProp.mass = massOverride;
	}
	if (!massProp.CheckValidity()) {
		DBG_Print("\nERROR: Bad mass properties on "
			// "%s%02d!\n", instance->object->name, instance->introNum);
			"%s!\n", instance->object->name);
		massProp.mass = 666.f;
		massProp.I = identity4x4 * 66600000.f;
	}
	Vector center = massProp.center;
	if (pMatCenterOfMass) {
		*pMatCenterOfMass = identity4x4;
		pMatCenterOfMass->m[3][0] = center.x;
		pMatCenterOfMass->m[3][1] = center.y;
		pMatCenterOfMass->m[3][2] = center.z;
	}
	// InitBodyPosition
	body->SetMassProperties(massProp);
	if (prop) {
		// InitBodyProperties(body, prop);
	}

	Vector3 ncenter = {-center};
	AddGeometriesToBody(body, hmodel, &ncenter, prop, instance);

	instance->multibodyComponent.m_body = body;
	instance->multibodyComponent.m_centerOfMass = massProp.center;
	return true;
}

