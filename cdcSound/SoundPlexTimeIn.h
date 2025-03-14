#pragma once
#include "SoundPlex.h"

namespace cdc {

class SoundPlexTimeIn : public SoundPlex {
	float m_delay;
	dtp::SoundPlex *m_snd;

public:
	SoundPlexTimeIn(
		float delay,
		dtp::SoundPlex *snd,
		Controls *controls,
		Controls3d *controls3d,
		SoundOwner *owner)
	:
		SoundPlex(controls, controls3d, owner),
		m_delay(delay),
		m_snd(snd)
	{
		// ---00010
		m_state &= ~0x1D;
		m_state |=  0x02;
	}

	SoundPlex *Update(float time) override {
		if ((m_state & 0xF) == 5) {
			delete this;
			return nullptr;
		}

		if (time == -1.f)
			return this;

		m_delay -= time;
		if (m_delay > 0.f)
			return this;

		if (m_snd == nullptr) {
			delete this;
			return nullptr;
		}

		SoundPlex *plex = SoundPlexCollection::Create(m_snd, m_controls, m_controls3d, m_owner);
		delete this;
		return plex;
	}

	void End(EndType) override {
		// ----0101
		m_state &= ~0xA;
		m_state |=  0x5;
	}
};

}
