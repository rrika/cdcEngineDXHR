#include "DX3Player.h"
#include "ObjectiveManager.h"

bool DX3Player::init() {
	// TODO
	if (!objectiveManager)
		objectiveManager = new ObjectiveManager(this);
	return true;
}

bool DX3Player::method_4() {
	objectiveManager->deactivateAll();
	// TODO
	return true;
}

bool DX3Player::uninit_a() {
	// TODO
	if (objectiveManager) {
		delete objectiveManager;
		objectiveManager = nullptr;
	}
	// TODO
	return true;
}

bool DX3Player::uninit_b() {
	// TODO
	return true;
}

// -------------------------------------------------------------------------- //

PlayerPair *PlayerPair::s_pair = nullptr;

void PlayerPair::init() {
	if (!s_pair)
		s_pair = new PlayerPair();
}

PlayerPair::PlayerPair() {
	playerIndex = 0;
	player[0] = new DX3Player();
	if (cdc::CdcPlayer::s_delegate)
		cdc::CdcPlayer::s_delegate->init(player[0]);
}

bool PlayerPair::unknown(cdc::CdcPlayer *player) {
	if (cdc::CdcPlayer::s_delegate)
		return cdc::CdcPlayer::s_delegate->method_4(player);
	return false;
}

void PlayerPair::uninit_b() {
	auto *player = s_pair->player[0];
	if (player && cdc::CdcPlayer::s_delegate)
		cdc::CdcPlayer::s_delegate->uninit_b(player);
}

void PlayerPair::uninit_a() {
	if (s_pair) {
		auto *player = s_pair->player[0];
		if (cdc::CdcPlayer::s_delegate)
			cdc::CdcPlayer::s_delegate->uninit_a(player);
		if (player)
			delete player;
		delete s_pair;
		s_pair = nullptr;
	}
}

// -------------------------------------------------------------------------- //

void Player::Init(Instance *instance, GameTracker*) {
	// TODO
	auto *cdcPlayer = PlayerPair::s_pair->getPlayer();
	auto *player = new Player();
	auto *api = new DXPlayerAPI(cdcPlayer);
	UserDataComponent::CreateIfNeeded(instance)->userData = player;
	cdcPlayer->instance = instance;
	cdcPlayer->player = player;
	cdcPlayer->playerAPI = api;
}

// -------------------------------------------------------------------------- //

class DX3PlayerDelegate : public cdc::CdcPlayer::IDelegate {
public:
	bool init(cdc::CdcPlayer *cdcPlayer) override { return static_cast<DX3Player*>(cdcPlayer)->init(); }
	bool method_4(cdc::CdcPlayer *cdcPlayer) override { return static_cast<DX3Player*>(cdcPlayer)->method_4(); }
	bool uninit_a(cdc::CdcPlayer *cdcPlayer) override { return static_cast<DX3Player*>(cdcPlayer)->uninit_a(); }
	bool uninit_b(cdc::CdcPlayer *cdcPlayer) override { return static_cast<DX3Player*>(cdcPlayer)->uninit_b(); }
};

void setupDX3PlayerDelegate() {
	static DX3PlayerDelegate delegate;
	cdc::CdcPlayer::s_delegate = &delegate;
}

// -------------------------------------------------------------------------- //

float DXPlayerAPI::getHealth() { return 100.f; } // TODO
