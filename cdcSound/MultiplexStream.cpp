#include <cstdio>
#include <cstring>
#include "MultiplexStream.h"
#include "Sample.h"

namespace cdc {

// see HackMultiplexStream.h
Sample *loadAndDemultiplex(STRHEADER*, const char *);

char MultiplexStreamImpl::s_soundStreamDir[256]; // line 136
char MultiplexStreamImpl::s_cinematicStreamDir[256]; // line 137

void MultiplexStream::SetSoundDirectory(const char *dir) { // line 323
	if (strlen(dir)+1 <= 256)
		strcpy(MultiplexStreamImpl::s_soundStreamDir, dir);
}

void MultiplexStream::SetCinematicDirectory(const char *dir) { // line 336
	if (strlen(dir)+1 <= 256)
		strcpy(MultiplexStreamImpl::s_cinematicStreamDir, dir);
}

bool MultiplexStreamImpl::Init(StreamType streamType, uint8_t priority, const char *name) { // line 2279 or 2284

	char mulname[256];
	char drmname[256];

	if (streamType == /*0*/ kSoundStream) {
		sprintf(mulname, "%s%s.mul", s_soundStreamDir, name);
		sprintf(drmname, "%s%s.drm", s_soundStreamDir, name);

	} else if (streamType == /*1*/ kCinematicStream) {
		sprintf(mulname, "%s%s.mul", s_cinematicStreamDir, name);
		sprintf(drmname, "%s%s.drm", s_cinematicStreamDir, name);
		// TODO
		
	} else if (streamType == /*2*/ kMusicStream) {
		sprintf(mulname, "%smusic\\%s\\%s.mul", "pc-w\\", name, name);
		sprintf(drmname, "%smusic\\%s\\%s.drm", "pc-w\\", name, name);
		// TODO
	}

	hackSample = loadAndDemultiplex(&m_streamHeader, mulname);

	return true;
}

MultiplexStream *MultiplexStream::CreateSoundStream( // line 2676
	const char *name,
	uint8_t priority
	// TODO
) {
	auto stream = new MultiplexStreamImpl();
	if (stream->Init(kSoundStream, priority, name))
		return stream;

	// TODO: cleanup
	return nullptr;
}

MultiplexStream *MultiplexStream::CreateMusicStream( // line 2729
	const char *name
) {
	auto stream = new MultiplexStreamImpl();
	if (stream->Init(kMusicStream, 110, name))
		return stream;

	// TODO: cleanup
	return nullptr;
}

}
