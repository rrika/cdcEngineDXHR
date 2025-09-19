#include <vector>
#include "cdcSound/cdcSound.h"
#include "cdcSound/Sample.h"
#include "cdcSound/SoundStreamHandler.h"
#include "cdcSound/Voice.h"

namespace cdc {

// see HackMultiplexStream.h
std::vector<Sample*> loadAndDemultiplex(STRHEADER*, const char *);

class SoundStreamHandlerImpl : public SoundStreamHandler { // line 142
public:
	struct Channel { // line 161
		Sample *sample = nullptr;                     //  4
		Voice *voice = nullptr;                       // 14
	};

private:
	friend class SoundStreamHandler;

	STRHEADER *m_streamHeader;         //  8
	Channel *m_channel;                //  C
	uint32_t m_channelCount;           // 10

	void CleanUp();

public:
	SoundStreamHandlerImpl(MediaStream *mediaStream);
	~SoundStreamHandlerImpl();
};

SoundStreamHandler::SoundStreamHandler(MediaStream *mediaStream) :
	MediaStreamHandler(mediaStream)
{
	// empty
}

SoundStreamHandlerImpl::SoundStreamHandlerImpl(MediaStream *mediaStream) : // line 411
	SoundStreamHandler(mediaStream)
{
	m_streamHeader = 0;
	m_channel = new Channel[12];
	m_channelCount = 0;
}

SoundStreamHandlerImpl::~SoundStreamHandlerImpl() { // line 450
	CleanUp();
}

SoundStreamHandler *SoundStreamHandler::Create(MediaStream *mediaStream) { // line 457
	return new SoundStreamHandlerImpl(mediaStream);
}

int SoundStreamHandler::ChannelCount() const { // line 474
	auto *impl = static_cast<SoundStreamHandlerImpl const*>(this);

	return impl->m_channelCount;
}

bool SoundStreamHandler::OnStreamStop() { // line 481
	auto *impl = static_cast<SoundStreamHandlerImpl*>(this);

	// TODO
	if (true /*m_isRunning*/) {

		for (uint32_t i=0; i<impl->m_channelCount; i++) {
			auto& channel = impl->m_channel[i];
			delete channel.voice;
			channel.voice = nullptr;
		}

		// m_isRunning = true;

		for (uint32_t i=0; i<impl->m_channelCount; i++) {
			if (impl->m_channel[i].sample->canReleaseImmediately() == false)
				return false;
		}
	}

	return true;
}


Voice *SoundStreamHandler::GetVoice(int channelIndex) { // line 1077
	auto *impl = static_cast<SoundStreamHandlerImpl*>(this);

	if (channelIndex < impl->m_channelCount)
		return impl->m_channel[channelIndex].voice;
	else
		return nullptr;
}

void SoundStreamHandler::hackLoad(STRHEADER *header, const char *path) {
	auto *impl = static_cast<SoundStreamHandlerImpl*>(this);

	impl->m_streamHeader = header;
	std::vector<Sample*> samples = loadAndDemultiplex(header, path);
	impl->m_channelCount = samples.size();
	for (uint32_t i=0; i<impl->m_channelCount; i++) {
		impl->m_channel[i].sample = samples[i];
	}
}

void SoundStreamHandler::Start() { // line 1143
	auto *impl = static_cast<SoundStreamHandlerImpl*>(this);

	for (uint32_t i=0; i<impl->m_channelCount; i++) {
		auto& channel = impl->m_channel[i];
		channel.voice = Voice::s_voiceCollection.Create(channel.sample);
		if (!channel.voice)
			break;
		// if (m_channelCount > 1)
		// 	voice->SyncWhenStarted();
		// static_cast<VoiceImpl*>(channel.voice)->SetControls(channel.controls, channel.controls3d);
		// static_cast<VoiceImpl*>(channel.voice)->setVolumeLeftRight(m_streamHeader->volLeft[i], m_streamHeader->volRight[i]);
	}
	// impl->m_isRunning = true;
}

void SoundStreamHandlerImpl::CleanUp() { // line 1397
	for (uint32_t i=0; i<m_channelCount; i++) {
		auto& channel = m_channel[i];
		if (channel.sample)
			channel.sample->Release();
		if (channel.voice)
			delete channel.voice;
		// TODO
	}
}

}
