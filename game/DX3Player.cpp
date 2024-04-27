#include "DX3Player.h"

bool DX3PlayerDelegate::init(cdc::CdcPlayer*) {
	// TODO
	return true;
}

bool DX3PlayerDelegate::method_4(cdc::CdcPlayer*) {
	// TODO
	return true;
}

bool DX3PlayerDelegate::uninit_a(cdc::CdcPlayer*) {
	// TODO
	return true;
}

bool DX3PlayerDelegate::uninit_b(cdc::CdcPlayer*) {
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

float DXPlayerAPI::getHealth() { return 100.f; } // TODO
