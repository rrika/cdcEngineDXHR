#pragma once
#include <list>
#include <string>
#include "cdcSound.h"
#include "cdcSys/RCObject.h"
#include "SoundOwner.h"

namespace dtp { struct SoundPlex; }

namespace cdc {

class SoundPlex;

class SoundPlexCollection : protected SoundTypes { // 25
	std::list<RCPtr<SoundOwner>> m_plexes;

public:
	static SoundPlex *Create(
		dtp::SoundPlex *snd,
		SoundTypes::Controls *controls,
		SoundTypes::Controls3d *controls3d,
		SoundOwner *owner
	);

	SoundHandle StartPaused(
		dtp::SoundPlex *snd,
		float delay
		// TODO
	);
};

class SoundPlex : protected SoundTypes { // 72
protected:
	uint32_t m_state; // 4
	Controls *m_controls; // 8
	Controls3d *m_controls3d; // C
	SoundOwner *m_owner; // 10

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

	virtual ~SoundPlex() = default; // 0
	virtual SoundPlex *Update(float) = 0; // 4
	// virtual uint8_t Method8(uint32_t) { return 0; }
	// virtual void MethodC(uint32_t, uint32_t) {}
	virtual void End(EndType) = 0; // 10

};

class SoundPlexSingleChild : public SoundPlex { // 146
protected:
	SoundPlex *m_plex;

public:
	SoundPlexSingleChild(
		Controls *controls,
		Controls3d *controls3d,
		SoundOwner *owner)
	:
		SoundPlex(controls, controls3d, owner)
	{}
};

SoundHandle SOUND_StartPaused(
	dtp::SoundPlex *snd,
	float delay
	// TODO
);

}

void buildUI(dtp::SoundPlex *snd, std::string indent="");
