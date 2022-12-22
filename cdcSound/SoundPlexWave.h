#pragma once
#include "cdc/dtp/soundplex.h"
#include "SoundPlex.h"

namespace cdc {

class Voice;

class SoundPlexWave : public SoundPlex {
	Voice *m_voice = nullptr; // 14

public:
	SoundPlexWave(
		dtp::SoundPlex::Wave *data,
		Controls *controls,
		Controls3d *controls3d,
		SoundOwner *owner);

	~SoundPlexWave() override;
	SoundPlex *Update(float) override;
	void End(EndType) override;
};

}
