#include <cstring>
#include "WaveSection.h"

namespace cdc {

Wave *WaveSection::lookupEntry(
	uint32_t sectionId, uint32_t unknown6, uint32_t size, bool& alreadyLoaded)
{
	if (sectionId < 4096) {
		auto &entry = entries[sectionId];
		entry.refCount++;
		if (entry.sample)
			alreadyLoaded = true;
		else {
			entry.size = size - 16; // minus the header
			entry.byteC = 100; // volume?
			loadingState = 0;
			alreadyLoaded = false;
		}

		return &entry;

	} else {
		alreadyLoaded = true;
		return nullptr;
	}
}

uint32_t WaveSection::StartResource(uint32_t sectionId, uint32_t unknown6, uint32_t size, bool& alreadyLoaded) {
	auto *entry = lookupEntry(sectionId, unknown6, size, alreadyLoaded);
	if (!alreadyLoaded && entry->refCount > 1)
		alreadyLoaded = true;

	return sectionId;
}

void WaveSection::HandleResourceData(uint32_t sectionId, void *vsrc, uint32_t size, uint32_t ignoredOffset) {
	if (size == 0)
		return;

	char *src = (char*)vsrc;
	Wave &entry = entries[sectionId];
	uint32_t offset = 0;
	uint32_t bytesLeftToProcess = size;
	while (size && bytesLeftToProcess) {
		switch (loadingState) {
		case 0:
			readTarget = (char*)&soundBlobHeader;
			readAmount = sizeof(soundBlobHeader);
			loadingState = 1;
			// fall through
		case 1:
			if (bytesLeftToProcess >= readAmount) {
				memcpy(readTarget, src + offset, readAmount);
				bytesLeftToProcess -= readAmount;
				offset += readAmount;

				entry.byteC = soundBlobHeader.dwordC;
				// TODO

				entry.sample = Sample::Create(0,
					entry.size,
					soundBlobHeader.loopStart,
					soundBlobHeader.loopEnd,
					soundBlobHeader.sampleRate);

				if (entry.sample) {
					loadingState = 2;
					loadingId = sectionId;
				} else {
					loadingState = 3;
				}

				totalSoundBytes = 0;
				break;

			} else {
				memcpy(readTarget, src + offset, bytesLeftToProcess);
				readTarget += bytesLeftToProcess;
				readAmount -= bytesLeftToProcess;
				return; // size;
			}

		case 2: {
			uint32_t availableBytes;
			if (ringbuffer.getBuffer()) {
				ringbuffer.push(src + offset, bytesLeftToProcess);
				availableBytes = ringbuffer.getAvailableBytes();
			} else
				availableBytes = bytesLeftToProcess;

			if (availableBytes == entry.size) {
				char *buffer = ringbuffer.getBuffer();
				if (!buffer)
					buffer = src + offset;
				entry.sample->Upload(totalSoundBytes, buffer, entry.size);
				// TODO
			}
		}

		case 3:
			return; // size;
		default:
			break;
		}
	}
	return; // size - bytesLeftToProcess;
}

void WaveSection::HandleResourceEnd(uint32_t id, void *drmSectionHeader) {
	loadingState = 0;
}

void *WaveSection::GetBasePointer(uint32_t sectionId) {
	return nullptr;
}

uint32_t WaveSection::FindResource(uint32_t sectionId) {
	if (entries[sectionId].refCount != 0)
		return sectionId;
	else
		return ~0u;
}

// called from SoundPlexWave::SoundPlexWave
Wave *WaveSection::WaveFind(uint32_t sectionId) {
	if (entries[sectionId].refCount != 0)
		return &entries[sectionId];
	return nullptr;
}

// HACK
void WaveSection::playSound(uint32_t sectionId) {
	if (auto *sample = entries[sectionId].sample)
		sample->Play();
}

}
