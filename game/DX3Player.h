#pragma once
#include <cstdint>
#include "cdcWorld/Player.h"
#include "cdcWorld/UserDataComponent.h"

class ObjectiveManager;
struct GameTracker;
struct Player;

class DX3Player : public cdc::CdcPlayer {
public:
	Player *player; // 8
	// ContextualActionManager *contextualActionManager = nullptr; // 24
	ObjectiveManager *objectiveManager = nullptr; // 30

	bool init();
	bool method_4();
	bool uninit_a();
	bool uninit_b();
};

void setupDX3PlayerDelegate();

class PlayerPair {
	DX3Player *player[2];
	int playerIndex;
public:
	PlayerPair();
	DX3Player *getPlayer0() { return player[0]; }
	DX3Player *getPlayer() { return player[playerIndex]; }

	static PlayerPair *s_pair;
	static void init();
	static bool unknown(cdc::CdcPlayer *player);
	static void uninit_b();
	static void uninit_a();
};

struct Player : InstanceUserData { // sizeof(Player) == 0x120
	uint32_t padding[0x54 / 4];
	// make sure offset 0x50 is readable in case someone mistakes this for an
	// UberObjectComposite or UberObjectSection
	static void Init(Instance*, GameTracker*);
};

class DXPlayerAPI : public cdc::PlayerAPI {
	DX3Player *cdcPlayer;
public:
	DXPlayerAPI(DX3Player *cdcPlayer) : cdcPlayer(cdcPlayer) {}
	float getHealth() override; // 4
	// method_8
	// ...
	// method_48
};
