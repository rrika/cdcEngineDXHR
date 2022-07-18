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

namespace cdc { class Scene; }

extern cdc::Scene *g_scene;

struct SceneLayer {
	static void init();
};
