#pragma once

namespace cdc {

class MediaStream {
public:
	// virtual float GetMediaTime(bool) = 0; //  0
	// virtual float GetMediaLength() = 0;   //  4
	// virtual bool HasCinematic() = 0;      //  8
	// virtual bool HasSubtitles() = 0;      //  C
	virtual void Play() = 0;              // 10
	virtual void Stop() = 0;              // 14
	// virtual void method18(...) = 0;
	// virtual void method1C(...) = 0;
	// virtual bool IsDone() = 0;            // 20
	// virtual void method24(...) = 0;
	virtual ~MediaStream() = default;     // 28
};

class MediaStreamHandler {
public:
	virtual void Start() = 0;            // 0
	virtual bool OnStreamStop() = 0;     // 4
	// virtual bool ReadyToPlay() = 0;      // 8
	// virtual bool AllDataProcessed() = 0; // C

protected:
	MediaStream *m_mediaStream;
	MediaStreamHandler(MediaStream *mediaStream) :
		m_mediaStream(mediaStream) {}
};

}
