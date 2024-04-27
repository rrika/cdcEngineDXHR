#pragma once
#include <cstdint>
#include "cdcWorld/Player.h"
#include "cdcWorld/UserDataComponent.h"

struct GameTracker;
struct Player;

class DX3Player : public cdc::CdcPlayer {
public:
	Player *player; // 8
	// ContextualActionManager *contextualActionManager24;
};

class DX3PlayerDelegate : public cdc::CdcPlayer::IDelegate {
public:
	bool init(cdc::CdcPlayer*) override;
	bool method_4(cdc::CdcPlayer*) override;
	bool uninit_a(cdc::CdcPlayer*) override;
	bool uninit_b(cdc::CdcPlayer*) override;
};

class PlayerPair {
	DX3Player *player[2];
	int playerIndex;
public:
	PlayerPair();
	DX3Player *getPlayer() { return player[playerIndex]; }

	static PlayerPair *s_pair;
	static void init();
	static bool unknown(cdc::CdcPlayer *player);
	static void uninit_b();
	static void uninit_a();
};

struct Player : InstanceUserData { // sizeof(Player) == 0x120
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
