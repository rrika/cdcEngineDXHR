#pragma once
#include <cstdint>

class Instance;
struct GameTracker;
struct Message;

using InitFunc = void(Instance*, GameTracker*);
using ProcessFunc = void(Instance*, GameTracker*);
using CollideFunc = void(Instance*, GameTracker*);
using QueryFunc = uintptr_t(Instance*, uint32_t);
using MessageFunc = void(Instance*, uint32_t, uintptr_t);
using DebugDrawFunc = void(Instance*, GameTracker*);
using objectMessageHandler = Message* (*)(Instance*, Message*);

struct ObjectFamily {
	InitFunc *init;
	InitFunc *uninit;
	ProcessFunc *process;
	CollideFunc *collide;
	CollideFunc *additionalCollide;
	QueryFunc *query;
	MessageFunc *message;
	DebugDrawFunc *debugDraw;
	void *unknown; // might be swapped with debugDraw
};
