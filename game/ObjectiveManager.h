#pragma once
#include <cstdint>
#include <list>
#include "cdcMath/Math.h"
#include "dtp/objective.h"

namespace cdc { class CdcPlayer; }

class Mission;
class Objective;
class ObjectiveManager;

enum class ObjectiveState {
	kNotAssigned = 0,
	kInProgress = 1,
	kCancelled = 2,
	kCompleted = 3,
	kFailed = 4,
	kDeleted = 5 // only partially implemented for missions
};

class Mission {
public:
	dtp::Mission *m_dtpMission = nullptr; // 0
	uint32_t m_dtpIndex; // 4
	std::list<Objective*> m_primaryObjectives; // 8
	std::list<Objective*> m_secondaryObjectives; // C
	ObjectiveState m_state; // 10
	uint32_t m_missionIndex; // 14
	uint32_t m_dword18;
	bool m_active; // 1C
	ObjectiveManager *m_objectiveManager;

	friend class ObjectiveManager;

public:
	Mission(ObjectiveManager*, uint32_t dtpIndex);
	~Mission() = default;

	const char *getName() const;
	const char *getDescription() const;
	int32_t getNameStringIndex() const;
	bool isInState(ObjectiveState) const;
	uint32_t getMissionIndex() const;
	bool isMainMission() const;
	const char *getState() const;

	void reload();
	void reloadIfInProgress();
	void registerObjective(Objective*);
	// void serialize(BinaryWriter&);
	// void deserialize(BinaryReader&);

	void insertInOrder(Objective*);
	void reinsertInOrder(Objective*);
	void stateTransition(ObjectiveState, bool);
};

struct UILocator {
	cdc::Vector position;
	bool isActive;
	uint32_t uiLocatorId;
};

class Objective {
	struct Locator {
		bool isActive;
		dtp::ObjectiveLocator *dtpLocator;
	};

	dtp::Objective *m_dtpObjective = nullptr; // 0
	uint32_t m_dtpIndex; // 4
	ObjectiveState m_state; // 8
	bool m_haveLocators; // C
	Mission *m_mission; // 10
	ObjectiveManager *m_objectiveManager; // 14
	uint32_t m_unitObjectId; // 18
	bool m_notSeen; // 1C
	bool m_noAlarm; // 1D

	Locator **m_locators; // 20, indexed by group first
	// uint32_t *m_ptr24; // unused?
	// uint32_t *m_ptr28; // unused?
	// uint32_t m_dword2C; // unused?
	UILocator *m_uiLocators; // 30

	friend class ObjectiveManager;

public:
	Objective(ObjectiveManager*, uint32_t dtpIndex);
	void load();
	void unload();
	void reload();
	const char *getName() const;
	const char *getDescription() const;
	bool isInState(ObjectiveState) const;
	bool isPrimaryObjective() const;
	Mission *getMission() const { return m_mission; }
	void forfeitNotSeenBonus(const char *groupName);
	void forfeitNoAlarmBonus(const char *groupName);
	void updateForScriptableLocator(uint32_t groupIndex, uint32_t uiLocatorId, bool active);
	bool shouldBeListedBefore(const Objective&) const;
	const char *getState();
	void stateTransition(ObjectiveState, bool force);
	UILocator *getLocators(uint32_t& numLocators);
	// void serialize(BinaryWriter&);
	// void deserialize(BinaryReader&);
};

class ObjectiveManager {
public:
	// float float0 = 0.0f;
	std::list<Mission*> m_missions; // 4
	std::list<Mission*> m_missionsActive; // 8
	std::list<Objective*> m_inProgress; // C
	std::list<Objective*> m_inProgressActive; // 10
	cdc::CdcPlayer *player; // 14

public:
	ObjectiveManager(cdc::CdcPlayer*);
	~ObjectiveManager();

	void loadAll();
	Objective *getOrCreateObjective(uint32_t dtpIndex);
	void startMission(Mission*);
	void endMission(Mission*);
	void forfeitNotSeenBonus(const char *groupName);
	void removeAllSideQuestsAndReloadIfInProgress();
	void deactivateAll();
	void uninit();
	Mission *getMission(uint32_t dtpIndex);
	void recomputeActiveObjectives();
	void moveMissionToFront(Mission*);
	Objective *getObjective(uint32_t dtpIndex);
	ObjectiveState getObjectiveState(uint32_t dtpIndex);
	void enterInProgress(Objective*);
	void leaveInProgress(Objective*);
	// void serialize(BinaryWriter&);
	// void deserialize(BinaryReader&);
	// static void message143(void *msgData);
};
