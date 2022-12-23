#include "SoundPlexAssignment.h"

namespace cdc {

SoundPlexAssignment::SoundPlexAssignment(
	dtp::SoundPlex::Assignment *data,
	Controls *controls,
	Controls3d *controls3d,
	SoundOwner *owner)
:
	SoundPlexSingleChild(controls, controls3d, owner)
{
	// TODO
	m_plex = SoundPlexCollection::Create(
		data->m_sound,
		controls /*TODO*/,
		controls3d /*TODO*/,
		m_owner);
}

SoundPlex *SoundPlexAssignment::Update(float time) {
	// TODO

	if (m_plex)
		m_plex = m_plex->Update(time);

	if (m_plex) {
		m_state = m_plex->m_state;
		return this;

	} else {
		delete this; // SoundPlexSingleChild::~SoundPlexSingleChild will free m_plex
		return nullptr;
	}
}

void SoundPlexAssignment::End(EndType end) {
	// TODO
	m_plex->End(end);
}

}