#pragma once

// class StreamingCallback {
// };

// class ImfStreamingManager {
// };

// struct CellGroupUserData {
// 	uint32_t dword0;
// 	uint16_t word4;
// 	uint16_t word6;
// 	void *unit8;
// 	uint32_t dwordC;
// 	uint32_t dword10;
// 	uint32_t dword14;
// 	uint8_t byte18;
// 	uint32_t dword1C;
// 	uint32_t dword20;
// 	float dword24;
// 	float dword28;
// 	float dword2C;
// 	float dword30;
// 	float dword34;
// };

namespace cdc { class CellStreamGroupData; class IScene; }
class Instance;
struct StreamUnit;

extern cdc::IScene *g_scene;

class StreamingCallback { // 57
public:
	virtual void UnitLoaded(StreamUnit*) = 0;
	virtual void UnitDumped(StreamUnit*) = 0;
	// virtual void method8();
	virtual void StreamGroupLoaded(cdc::CellStreamGroupData*) = 0;
	virtual void StreamGroupDumped(cdc::CellStreamGroupData*) = 0;
	// virtual void method14();
};

struct SceneLayer { // 192
	static void init();
	static void AddStreamingCallback(StreamingCallback*);
	static void RemoveStreamingCallback(StreamingCallback*);
	static void PreStreamIn(StreamUnit*);
	static void PostStreamIn(StreamUnit*);
	static void AddInstance(Instance *instance);
	static void RemoveInstance(Instance *instance);
	static void Update();
};
