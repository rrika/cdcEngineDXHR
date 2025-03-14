#pragma once
#include <cstdint>

namespace dtp {

struct Controls3d { // line 81
	float distanceOuter; // 0
	float distanceInner; // 4
	float pan; // 8
		// float levelOfWide3D; // when playbackType == 3
		// float m_2DfrontPan; // when playbackType == 6
	float levelOfLFE; // C
	float float10;
	float float14;
	uint32_t group; // 18
	uint16_t playbackType; // 1C
		// lower eight bit are dtp::SoundPlaybackType
		// see VoiceImpl::Update3D
	float position[4]; // 20
};

}
