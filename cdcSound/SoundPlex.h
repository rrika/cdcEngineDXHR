#pragma once
#include "cdcSound.h"
#include "SoundOwner.h"

namespace dtp { struct SoundPlex; }

namespace cdc {

class SoundPlex;

class SoundPlexCollection : protected SoundTypes { // 25
	static SoundPlex *Create(
		dtp::SoundPlex *snd,
		SoundTypes::Controls *controls,
		SoundTypes::Controls3d *controls3d,
		SoundOwner *owner
	);
};

class SoundPlex : protected SoundTypes { // 72
protected:
	Controls *m_controls;
	Controls3d *m_controls3d;
	SoundOwner *m_owner;

public:
	SoundPlex(
		Controls *controls,
		Controls3d *controls3d,
		SoundOwner *owner)
	:
		m_controls(controls),
		m_controls3d(controls3d),
		m_owner(owner)
	{}
};

class SoundPlexSingleChild : public SoundPlex { // 146
protected:
	SoundPlex *m_plex;
};

}
