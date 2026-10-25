#include <cstring>
#include "srmusic.h"
#include "cdcFile/FileHelpers.h"
#include "cdcFile/FileReceiver.h"
#include "cdcFile/FileSystem.h"
#include "cdcGameShell/cdcGameShell.h"
#include "cdcSound/MultiplexStream.h"
#include "cdcSys/Assert.h"

using namespace cdc;

struct SRMUSIC_MusicSlot {
	char m_streamFullPath[256];          // 400 .mul
	cdc::MultiplexStream *m_soundStream; // 520

	void Free();
	void LoadMusic(const char *name, int fadingDownFrames, int fadingUpFrames, bool instantStart);
	void StopMusic();
	void Reset();
	void StartMusicLoad(const char *musicName, bool instantStart);
};

static constexpr size_t kNumMusicSlots = 8;
SRMUSIC_MusicSlot g_musicSlots[kNumMusicSlots];

void SRMUSIC_MusicSlot::Free() { // line 146
	// HACK
	if (m_soundStream) {
		m_soundStream->Stop();
		delete m_soundStream;
	}
	Reset();
}

void SRMUSIC_MusicSlot::LoadMusic(const char *name, int fadingDownFrames, int fadingUpFrames, bool instantStart) { // line 226
	// HACK
	Free();
	StartMusicLoad(name, instantStart);
}

void SRMUSIC_MusicSlot::StopMusic() { // line 368
	// TODO
	if (m_soundStream) // HACK
		Free();
}

void SRMUSIC_MusicSlot::Reset() { // line 399
	// TODO
	m_soundStream = nullptr;
}

void SRMUSIC_MusicSlot::StartMusicLoad(const char *musicName, bool instantStart) { // line 442

	GameShell::LOAD_MusicFileName(m_streamFullPath, musicName);
	strcat(m_streamFullPath, ".mul");

	if (getDefaultFileSystem()->FileExists(m_streamFullPath)) {
		m_soundStream = MultiplexStream::CreateMusicStream(musicName);
		m_soundStream->Play(); // HACK
	}
	else
		FatalError("Cannot load music files for the '%s' music!\nPlease rebuild music.", musicName);
}

void SRMUSIC_LoadMusic(int slotIndex, const char *name, int fadingDownFrames, int fadingUpFrames) { // line 658
	// TODO: if (!nomusic && !nosound)
	g_musicSlots[slotIndex].LoadMusic(name, fadingDownFrames, fadingUpFrames, true);
}

void SRMUSIC_StopMusic() { // line 677
	for (auto i=0u; i<kNumMusicSlots; i++) {
		g_musicSlots[i].StopMusic();
	}
}

#include "config.h"
#if ENABLE_IMGUI
#include <cstdio>
#include "imgui/imgui.h"
#include "cdcSound/SoundStreamHandler.h"
#include "cdcSound/Voice.h"

void SRMUSIC_buildMenu() {
	for (auto i=0u; i<kNumMusicSlots; i++)
		if (g_musicSlots[i].m_soundStream)
			goto menu_visible;
	return;
menu_visible:
	if (!ImGui::BeginMenu("Music player"))
		return;
	for (auto i=0u; i<kNumMusicSlots; i++) {
		auto& slot = g_musicSlots[i];
		if (slot.m_soundStream) {
			Voice *v = static_cast<MultiplexStreamImpl*>(slot.m_soundStream)->GetSoundStreamHandler()->GetVoice(0);
			auto *vi = static_cast<VoiceImpl*>(v);

			FMOD::Channel *channel = vi->m_channel;
			FMOD::Sound *currentSound = nullptr;
			if (channel->getCurrentSound(&currentSound) == FMOD_OK) {

				unsigned pos = 0;
				vi->m_channel->getPosition(&pos, FMOD_TIMEUNIT_MS);
				pos /= 1000;
				auto min = pos / 60;
				auto sec = pos % 60;

				unsigned total = 0;
				currentSound->getLength(&total, FMOD_TIMEUNIT_MS);
				total /= 1000;
				auto totalMin = total / 60;
				auto totalSec = total % 60;

				char time[20] = {0};
				snprintf(time, 19, "%d:%02d / %d:%02d",
					min, sec, totalMin, totalSec);

				if (ImGui::MenuItem(slot.m_streamFullPath, time))
					slot.StopMusic();

			} else {
				// unsure when this happens, but offer to stop it anyway
				if (ImGui::MenuItem(slot.m_streamFullPath))
					slot.StopMusic();
			}
		} else {
			ImGui::PushID(i);
			ImGui::MenuItem("(empty)", nullptr, nullptr, false);
			ImGui::PopID();
		}
	}
	ImGui::EndMenu();
}
#else
void SRMUSIC_buildMenu() {}
#endif
