#pragma once
#include <cstdint>

namespace dtp {

struct ObjectiveLocator {
	uint32_t introId; // 0
	bool startActive; // 4
	bool suppressDefaultLocator; // 5
};

struct ObjectiveLocatorGroup {
	uint32_t introId; // 0
	uint32_t mode; // 4
	uint32_t numStreamUnitIds; // 8
	uint16_t *streamUnitIds; // C
	uint32_t numLocators; // 10
	ObjectiveLocator *locators; // 14
};

struct ObjectiveRestrictedDomain {
	uint32_t dword0;
	const char *groupName; // hypothesis: trigger group name or ai group name
};

struct Objective {
	uint16_t titleStringIndex; // 0
	uint16_t descriptionStringIndex; // 2
	uint8_t quickSaveUponCompletion; // 4
	uint8_t isSecondaryObjective; // 5
	uint32_t missionDtpId; // 8
	uint32_t completionXP; // C
	uint32_t completionMoney; // 10
	uint8_t noSeenBonusAvailable; // 14
	uint32_t notSeenXP; // 18
	uint8_t noAlarmBonusAvailable; // 1C
	uint32_t noAlarmXP; // 20
	ObjectiveRestrictedDomain *restrictedDomains;
	uint32_t numRestrictedDomains;
	uint32_t numLocatorGroups; // 2C
	ObjectiveLocatorGroup *locatorGroups; // 30
};

struct Mission {
	static constexpr uint16_t kSideQuest = 1;
	static constexpr uint16_t kSideQuestOnlyForced = 2;

	uint16_t titleStringIndex; // 0
	uint16_t descriptionStringIndex; // 2
	uint16_t flags; // 4
};

}
