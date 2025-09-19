#include <cstdio>
#include <cstring>
#include <vector>
#include "cdcFile/ArchiveFileSystem.h"
#include "cdcFile/FileHelpers.h"
#include "cdcSound/MultiplexStream.h"
#include "cdcSound/Sample.h"

using namespace std;

namespace cdc {

vector<Sample*> loadAndDemultiplex(STRHEADER *header, const char *path) {
	auto fs = archiveFileSystem_default; // HACK
	uint32_t size = fs->getSize(path);
	char *buffer = FSHelper_ReadFile(path);
	char *end = buffer + size;

	memcpy(header, buffer, sizeof(STRHEADER));
	char *cursor = buffer + 0x800;

	vector<char> audioStream;
	vector<char> facefxStream;
	vector<char> subtitleStream;

	while (cursor < end) {
		uint32_t *header = (uint32_t *)cursor;
		uint32_t segmentType = header[0];
		uint32_t segmentSize = header[1];

		// printf("segment [%x] %d %x\n", cursor-buffer, segmentType, segmentSize);
		cursor += 16;

		vector<char> *v;
		if (segmentType == 0)      v = &audioStream;
		else if (segmentType == 1) v = &facefxStream;
		else if (segmentType == 2) v = &subtitleStream;

		v->insert(v->end(), cursor, cursor + segmentSize);

		cursor += (segmentSize + 15) & ~15;
	}

	char *audioBegin = audioStream.data();
	char *audioCursor = audioStream.data();
	char *audioEnd = audioStream.data() + audioStream.size();

	vector<vector<char>> streams;

	for (uint32_t i = 0; i < header->channelCount; i++)
		streams.emplace_back();

	while (audioCursor < audioEnd) {
		uint32_t *header = (uint32_t *)audioCursor;
		uint32_t blockSize = header[0];
		uint32_t blockChannel = header[1];

		// printf("block [%x/%x] %d %x\n", audioCursor-audioBegin, audioEnd-audioBegin, blockChannel, blockSize);
		audioCursor += 16;

		streams[blockChannel].insert(streams[blockChannel].end(), audioCursor, audioCursor + blockSize);

		audioCursor += blockSize;
	}

	uint32_t startLoop = header->startLoop;
	uint32_t endLoop = header->endLoop;

	if (startLoop == -1) {
		startLoop = 0;
		endLoop = 0;
	}

	// printf("Sample::Create size=%x startLoop=%d endLoop=%d hertz=%d\n", streams[0].size(), startLoop, endLoop, header->hertz);
	// printf("first bytes: %02x %02x %02x %02x\n", streams[0][0], streams[0][1], streams[0][2], streams[0][3]);

	// only consider channel 0
	vector<Sample*> samples;
	for (uint32_t i = 0; i < header->channelCount; i++) {
		Sample *s = Sample::Create(0, streams[i].size(), startLoop, endLoop, header->hertz);
		s->Upload(0, streams[i].data(), 0);
		samples.push_back(s);
	}

	delete[] buffer;
	return samples;
}

}
