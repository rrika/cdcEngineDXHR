#pragma once
#include <cstdint>
#include "cdcPhysics/Multibody.h"
#include "cdcWorld/InstanceComponent.h"

namespace cdc {

class ProxyComponent : public InstanceComponent {
public:
	void InitCommon(/*...*/);
	// uint32_t dword0;
	// uint8_t gap4[1612];
	// __m128 oword650;
	// float float660;
	// float float664;
	// float float668;
	// float capsuleRadius66C;
	// float capsuleLength670;
	// uint32_t dword674;
	// uint8_t gap678[8];
	// __m128 m_DTPAuthoredPositionOffset_maybe_680;
	// __m128 oword690;
	// __m128 m128_6A0;
	// __m128 quat6B0;
	// float nextCapsuleRadius6C0;
	// float nextCapsuleLength6C4;
	// uint32_t dword6D0;
	// uint32_t dword6D4;
	// uint32_t dword6D8;
	// uint32_t dword6DC;
	// __m128 m128_6E0;
	// __m128 oword6F0;
	// __m128 oword700;
	// float float710;
	// float float714;
	// Instance *pinstance718;
	// uint32_t dword71C;
	// uint32_t dword720;
	// uint32_t dword724;
	// PhysicsBody *body728;
	// PhysicsGeometry *geom72C;
	PhysicsJoint *joint730;
	// float float734;
	// uint32_t dword738;
	// uint8_t byte73C;
	// uint8_t byte73D;
	// uint8_t byte73E;
	// uint8_t byte73F;
	// uint8_t byte740;
	// uint32_t dword744;
	// uint32_t count748;
	// uint32_t dword74C;
	// uint32_t dword750;
	// uint32_t dword754;
};

}
