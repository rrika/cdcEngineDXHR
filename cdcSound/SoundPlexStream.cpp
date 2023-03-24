#include <cstdio>
#include "Sample.h"
#include "SoundPlexStream.h"

// HACK
extern uint32_t subtitleIndex;

namespace cdc {

SoundPlexStream::SoundPlexStream(
	void *data,
	Controls *controls,
	Controls3d *controls3d,
	SoundOwner *owner)
:
	SoundPlexSingleChild(controls, controls3d, owner)
{
	dtpStream = (dtp::SoundPlex::Stream *)data;
	m_state &= ~15;
	m_state |=   1;
	// TODO
}

uint32_t SoundPlexStream::Init() {
	printf("SoundPlexStream::Init this=%p, dtpStream=%p, name=%s\n", this, dtpStream, dtpStream ? dtpStream->m_streamName : nullptr);
	// TODO
	multiplexStream = MultiplexStreamImpl::CreateSoundStream(
		dtpStream->m_streamName,
		dtpStream->m_priorityStream
		// TODO
	);

	subtitleIndex = dtpStream->stringIndex12;

	// HACK
	((MultiplexStreamImpl*)multiplexStream)->hackSample->Play();

	m_state &= ~0xD;
	m_state |= 0x12;

	return true;
}

SoundPlex *SoundPlexStream::Update(float) {
	// TODO

	if ((m_state & 0xF) == 1) {
		// TODO
		Init();
	}

	return nullptr;
}

void SoundPlexStream::End(EndType) {
	// TODO
}

}
