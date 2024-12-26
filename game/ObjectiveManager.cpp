#include <cstdio>
#include <cstring>
#include <span>
#include "ObjectiveManager.h"
#include "cdcLocale/localstr.h"
#include "cdcResource/DTPDataSection.h"
#include "cdcSave/BinaryReaderWriter.h"
#include "cdcWorld/Instance.h"
#include "cdcWorld/Intro.h"
#include "cdcWorld/stream.h"
#include "cdcWorld/StreamUnit.h"
#include "cdc/dtp/objectproperties/intro.h"

#ifdef _WIN32
#define strcasecmp _stricmp
#endif

using namespace cdc;

static const char *missionStateToString(ObjectiveState state) {
	using enum ObjectiveState;
	switch (state) {
		case kNotAssigned: // 0
			return "not assigned";
		case kInProgress: // 1
			return "in progress";
		case kCancelled: // 2
			return "cancelled";
		case kCompleted: // 3
			return "completed";
		case kFailed: // 4
			return "failed";
		// unclear why "deleted" is missing here
		// the script bindings in NtMission allow setting state to deleted
		default:
			return "undefined";
	}
}

static const char *objectiveStateToString(ObjectiveState state) {
	using enum ObjectiveState;
	switch (state) {
		case kNotAssigned: // 0
			return "not assigned";
		case kInProgress: // 1
			return "in progress";
		case kCancelled: // 2
			return "cancelled";
		case kCompleted: // 3
			return "completed";
		case kFailed: // 4
			return "failed";
		case kDeleted: // 5
			return "deleted";
		default:
			return "undefined";
	}
}

Mission::Mission(ObjectiveManager *objectiveManager, uint32_t dtpIndex) {
	m_dtpIndex = dtpIndex;
	m_state = ObjectiveState::kNotAssigned;
	m_missionIndex = 0;
	m_active = false;
	m_dword18 = 1;
	m_objectiveManager = objectiveManager;
	m_dtpMission = (dtp::Mission*) DTPDataSection::getPointer(dtpIndex);
}

const char *Mission::getName() const {
	if (m_dtpMission)
		if (auto name = localstr_get(m_dtpMission->titleStringIndex))
			return name;
	return "(no name)";
}

const char *Mission::getDescription() const {
	if (m_dtpMission)
		if (auto desc = localstr_get(m_dtpMission->descriptionStringIndex))
			return desc;
	return "(no description)";
}

int32_t Mission::getNameStringIndex() const {
	if (m_dtpMission)
		return m_dtpMission->titleStringIndex;
	return -1;
}

bool Mission::isInState(ObjectiveState state) const {
	return m_state == state;
}

uint32_t Mission::getMissionIndex() const {
	using enum ObjectiveState;
	switch (m_state) {
	case kCancelled:
	case kCompleted:
	case kFailed:
		return 0;
	case kNotAssigned:
	case kInProgress:
	case kDeleted:
	default:
		return m_missionIndex;
	}
}

bool Mission::isMainMission() const {
	if (m_dtpMission)
		return (m_dtpMission->flags & dtp::Mission::kSideQuest) == 0;
	return false;
}

const char *Mission::getState() const {
	return missionStateToString(m_state);
}

void Mission::reload() {
	for (Objective *objective : m_primaryObjectives)
		objective->reload();
	for (Objective *objective : m_secondaryObjectives)
		objective->reload();
}

void Mission::reloadIfInProgress() {
	if (m_state == ObjectiveState::kInProgress)
		reload();
}

void Mission::registerObjective(Objective *objective) {
	if (objective->isPrimaryObjective())
		m_primaryObjectives.push_front(objective);
	else
		m_secondaryObjectives.push_front(objective);
}

void Mission::serialize(BinaryWriter&) { /*TODO*/ }
void Mission::deserialize(BinaryReader&) { /*TODO*/ }

void Mission::insertInOrder(Objective *objective) {
	auto& objectiveList = objective->isPrimaryObjective()
		? m_primaryObjectives
		: m_secondaryObjectives;

	for (auto it=objectiveList.begin(); it != objectiveList.end(); it++) {
		if (objective->shouldBeListedBefore(**it)) {
			objectiveList.insert(it, objective); // or is it insert after?
			return;
		}
	}

	objectiveList.push_front(objective); // push_back makes more sense, but that's what it is
}

void Mission::reinsertInOrder(Objective *objective) {
	auto& objectiveList = objective->isPrimaryObjective()
		? m_primaryObjectives
		: m_secondaryObjectives;

	for (auto it=objectiveList.begin(); it != objectiveList.end(); it++)
		if (*it == objective) {
			objectiveList.erase(it);
			break;
		}

	insertInOrder(objective);
}

void Mission::stateTransition(ObjectiveState targetState, bool force) {
	using enum ObjectiveState;

	if (m_state == targetState)
		return; // nothing to do

	if (m_dtpMission->flags & dtp::Mission::kSideQuestOnlyForced) {
		if (isMainMission())
			return; // main mission can't transition
		else if (!force)
			return; // side mission can only transition when forced
	}

	if (targetState == kCompleted) {
		bool allDone = true;
		for (Objective *objective : m_primaryObjectives)
			if (!objective->isInState(kCompleted) &&
				!objective->isInState(kCancelled) &&
				!objective->isInState(kDeleted))
				allDone = false;

		if (force && !isMainMission())
			allDone = true; // who cares about prerequisites

		if (!allDone)
			return;
	}

	m_state = targetState;

	// assign incrementing mission index
	if (targetState == kInProgress) {
		m_objectiveManager->startMission(this);
		// TODO:
		// NsPlayerController *c = ...;
		static int NsPlayerController_mainMissionCount = 0;
		static int NsPlayerController_sideMissionCount = 0;
		if (isMainMission())
			m_missionIndex = NsPlayerController_mainMissionCount++;
		else
			m_missionIndex = NsPlayerController_sideMissionCount++;
	} else {
		m_objectiveManager->endMission(this);
		m_missionIndex = 0;
	}

	// cancel associated objectives
	if (targetState == kCancelled) {
		for (Objective *objective : m_primaryObjectives)
			if (objective->isInState(kInProgress))
				objective->stateTransition(kCancelled, false);

		for (Objective *objective : m_secondaryObjectives)
			if (objective->isInState(kInProgress))
				objective->stateTransition(kCancelled, false);
	}
}

//

Objective::Objective(ObjectiveManager *objectiveManager, uint32_t dtpIndex) {

	m_dtpIndex = dtpIndex;
	m_objectiveManager = objectiveManager;
	m_unitObjectId = ~0u;
	// m_ptr24 = nullptr;
	// m_ptr28 = nullptr;
	m_uiLocators = nullptr;
	m_dtpObjective = (dtp::Objective*) DTPDataSection::getPointer(dtpIndex);
	m_notSeen = m_dtpObjective->noSeenBonusAvailable;
	m_noAlarm = m_dtpObjective->noAlarmBonusAvailable;
	m_mission = objectiveManager->getMission(m_dtpObjective->missionDtpId);
	// m_state, m_haveLocators and m_locators is initialized by load()

	m_mission->registerObjective(this);
	load();
}

void Objective::load() {
	uint32_t numMaxLocators = 0;
	m_state = ObjectiveState::kNotAssigned;
	m_locators = new Locator*[m_dtpObjective->numLocatorGroups];
	if (m_dtpObjective->numLocatorGroups)
		numMaxLocators = 1;

	for (uint32_t i=0; i<m_dtpObjective->numLocatorGroups; i++) {
		auto *group = &m_dtpObjective->locatorGroups[i];
		if (numMaxLocators < group->numLocators)
			numMaxLocators = group->numLocators;

		m_locators[i] = new Locator[group->numLocators];
		for (uint32_t j=0; j<group->numLocators; j++) {
			m_locators[i][j].isActive = group->locators[j].startActive;
			m_locators[i][j].dtpLocator = &group->locators[j];
		}
	}

	m_haveLocators = false;
	if (numMaxLocators > 0) {
		// m_ptr28 = new uint128_t[numMaxLocators];
		// m_ptr24 = new uint32_t[numMaxLocators];
		m_uiLocators = new UILocator[numMaxLocators];
		m_haveLocators = true;
	}

}

void Objective::unload() {
	// delete[] m_ptr28; m_ptr28 = nullptr;
	// delete[] m_ptr24; m_ptr24 = nullptr;
	for (uint32_t i=0; i<m_dtpObjective->numLocatorGroups; i++)
		delete[] m_locators[i];
	delete[] m_locators; m_locators = nullptr;
	delete[] m_uiLocators; m_uiLocators = nullptr;
}

void Objective::reload() {
	unload();
	load();
}

const char *Objective::getName() const {
	if (m_dtpObjective)
		if (auto name = localstr_get(m_dtpObjective->titleStringIndex))
			return name;
	return "(no name)";
}

const char *Objective::getDescription() const {
	if (m_dtpObjective)
		if (auto desc = localstr_get(m_dtpObjective->descriptionStringIndex))
			return desc;
	return "(no description)";
}

bool Objective::isInState(ObjectiveState state) const {
	return m_state == state;
}

bool Objective::isPrimaryObjective() const {
	if (m_dtpObjective)
		return (m_dtpObjective->isSecondaryObjective & 1) == 0;
	return false;
}

void Objective::forfeitNotSeenBonus(const char *groupName) {
	if (groupName && m_notSeen && m_state == ObjectiveState::kInProgress && m_dtpObjective) {
		for (uint32_t i=0; i<m_dtpObjective->numRestrictedDomains; i++) {
			if (strcasecmp(m_dtpObjective->restrictedDomains[i].groupName, groupName) == 0) {
				m_notSeen = false;
				break;
			}
		}
	}
}

void Objective::forfeitNoAlarmBonus(const char *groupName) {
	if (groupName && m_noAlarm && m_state == ObjectiveState::kInProgress && m_dtpObjective) {
		for (uint32_t i=0; i<m_dtpObjective->numRestrictedDomains; i++) {
			if (strcasecmp(m_dtpObjective->restrictedDomains[i].groupName, groupName) == 0) {
				m_noAlarm = false;
				break;
			}
		}
	}
}

void Objective::updateForScriptableLocator(uint32_t groupIndex, uint32_t uiLocatorId, bool active) {
	Locator *l = &m_locators[groupIndex][uiLocatorId];
	// TODO: send message 10
}

bool Objective::shouldBeListedBefore(const Objective& other) const {
	using enum ObjectiveState;
	if (m_state == kNotAssigned)
		return false;
	if (m_state == kInProgress)
		return true;
	if (m_state == kDeleted)
		return false;
	if (other.isInState(kInProgress))
		return false; // cancelled, completed or failed objective yields to other in-progress objective
	return true;
}

const char *Objective::getState() {
	return objectiveStateToString(m_state);
}

void Objective::stateTransition(ObjectiveState targetState, bool force) {
	using enum ObjectiveState;

	if (m_state == targetState)
		return;

	if (force || (m_state == kNotAssigned || m_state == kInProgress) && targetState != kNotAssigned) {
		if (targetState == kInProgress)
			m_objectiveManager->enterInProgress(this);

		else if (m_state == kInProgress)
			m_objectiveManager->leaveInProgress(this);

		m_state = targetState;

		if (m_state != kNotAssigned)
			m_mission->reinsertInOrder(this);

		if (isPrimaryObjective() && targetState != kCancelled && targetState != kDeleted) {
			m_mission->stateTransition(targetState, false);
			m_objectiveManager->moveMissionToFront(m_mission);
		}

		// TODO: send message 9

		if (targetState == kCompleted) {
			if (m_dtpObjective->completionMoney) {
				// TODO: add money
			}

			if (m_mission->isMainMission()) {
				if (isPrimaryObjective()) {
					// TODO: award experience points
				} else {
					// TODO: award experience points
				}
			} else {
				// TODO: award experience points
			}

			// TODO: send message 6 / telemetry
			
			if (m_notSeen) {
				// TODO: award experience points
			}

			if (m_noAlarm) {
				// TODO: award experience points
			}
		}

		if (m_dtpObjective->quickSaveUponCompletion) {
			if (targetState == kCompleted || targetState == kFailed) {
				// TODO: quick save
			}
		}

	} else {
		printf("Error : an objective can't go from [%s] to [%s]",
			objectiveStateToString(m_state),
			objectiveStateToString(targetState));
	}
}

static bool getPositionByUniqueID(uint32_t id, Vector& pos) {
	if (id == 0 || id == ~0u)
		return false;

	if (Instance *instance = Instance::Find(id)) {
		/* if (OBJ_GetFamily(instance) == 83) {
			// TODO
		} else */ {
			pos = instance->position;
		}
		return true;
	}

	if (dtp::Intro *intro = IntroManager::FindIntroUniqueID(id, nullptr)) {
		auto p = intro->position;
		pos = {p[0], p[1], p[2], p[3]};
		return true;
	}

	return false;
}

UILocator *Objective::getLocators(uint32_t& outNumLocators) {
	if (!m_dtpObjective || !m_haveLocators || !m_uiLocators) {
		outNumLocators = 0;
		return m_uiLocators;
	}

	uint32_t matchingGroup = ~0u;
	uint32_t fallbackGroup = ~0u;

	// find group containing loaded player spawn unit

	for (uint32_t i = 0; i < m_dtpObjective->numLocatorGroups; i++) {
		auto *group = &m_dtpObjective->locatorGroups[i];

		if (group->mode == 0)
			fallbackGroup = i;

		if (group->mode == 1) {
			uint32_t playerSpawnUnitID = 0x99999999; // TODO
				// m_objectiveManager->player->instance->streamComponent.m_birthStreamUnitID;
			for (uint32_t unitID : std::span(group->streamUnitIds, group->numStreamUnitIds)) {
				auto *unit = STREAM_GetStreamUnitWithID(unitID);
				if (unitID == playerSpawnUnitID && unit && unit->used /*== 2*/) {
					matchingGroup = i;
					break;
				}
			}
		}
	}

	// find group containing any loaded unit

	if (matchingGroup == ~0u) {
		for (uint32_t i = 0; i < m_dtpObjective->numLocatorGroups; i++) {
			auto *group = &m_dtpObjective->locatorGroups[i];

			if (group->mode == 1) {
				for (uint32_t unitID : std::span(group->streamUnitIds, group->numStreamUnitIds)) {
					auto *unit = STREAM_GetStreamUnitWithID(unitID);
					if (unit && unit->used /*== 2*/) {
						matchingGroup = i;
						break;
					}
				}
			}
		}
	}

	UILocator *uiLocator = m_uiLocators;

	if (m_unitObjectId == ~0u) {
		uint32_t chosenGroup;
		if (matchingGroup != ~0u)
			chosenGroup = matchingGroup;
		else if (fallbackGroup != ~0u)
			chosenGroup = fallbackGroup;
		else {
			outNumLocators = 0;
			return m_uiLocators;
		}

		auto *dtpGroup = &m_dtpObjective->locatorGroups[chosenGroup];
		bool suppressDefaultLocator = false;

		for (uint32_t i=0; i<dtpGroup->numLocators; i++) {
			auto *locator = &m_locators[chosenGroup][i];
				
			if (Vector pos; getPositionByUniqueID(locator->dtpLocator->introId, pos)) {
				suppressDefaultLocator |= locator->dtpLocator->suppressDefaultLocator;
				*uiLocator++ = {
					/*position=*/ pos,
					/*isActive=*/ locator->isActive,
					/*uiLocatorId=*/ (i+1) << 16 | (chosenGroup+1) & 0xffff
				};
			}
		}

		if (Vector pos; getPositionByUniqueID(dtpGroup->introId, pos) && !suppressDefaultLocator) {
			*uiLocator++ = {
				/*position=*/ pos,
				/*isActive=*/ true,
				/*uiLocatorId=*/ 0xffff0000 | (chosenGroup+1) & 0xffff
			};
		}

	} else if (Vector pos; getPositionByUniqueID(m_unitObjectId, pos)) {
		*uiLocator++ = {
			/*position=*/ pos,
			/*isActive=*/ true,
			/*uiLocatorId=*/ ~0u
		};
	}

	outNumLocators = uiLocator - m_uiLocators;
	return m_uiLocators;
}

void Objective::serialize(BinaryWriter&) { /*TODO*/ }
void Objective::deserialize(BinaryReader&) { /*TODO*/ }

//

ObjectiveManager::ObjectiveManager(CdcPlayer *player) : player(player) {
	// TODO: register message143
}

ObjectiveManager::~ObjectiveManager() {
	// TODO: unregister message143
}

extern void *objectiveDatabase; // see Main.cpp

void ObjectiveManager::loadAll() {
	uint32_t *db = (uint32_t*)objectiveDatabase;
	uint32_t numObjectives = *db++; // count, dtpIndex0, dtpIndex1, ...
	for (uint32_t i=0; i<numObjectives; i++)
		new Objective(this, *db++);
}

Objective *ObjectiveManager::getOrCreateObjective(uint32_t dtpIndex) {
	if (auto *objective = getObjective(dtpIndex))
		return objective;

	return new Objective(this, dtpIndex);
}

void ObjectiveManager::startMission(Mission *mission) {
	// copy 1 mission from       m_mission    -> m_missionsActive
	// copy N objectives from    m_inProgress -> m_inProgressActive

	for (Mission *activeMission : m_missionsActive)
		if (activeMission == mission)
			return;

	m_missionsActive.push_front(mission);
	mission->m_active = true;

	for (Objective *inProgress : m_inProgress)
		if (inProgress->getMission() == mission)
			m_inProgressActive.push_front(inProgress);
}

void ObjectiveManager::endMission(Mission *mission) {
	for (auto it = m_missionsActive.begin(); it != m_missionsActive.end(); it++) {
		if (*it != mission)
			continue;
		m_inProgressActive.remove_if([=](Objective *objective) { return objective->m_mission == mission; });
		return;
	}
}

void ObjectiveManager::forfeitNotSeenBonus(const char *groupName) {
	for (Objective *inProgress : m_inProgress)
		inProgress->forfeitNotSeenBonus(groupName);
}

void ObjectiveManager::removeAllSideQuestsAndReloadIfInProgress() {
	m_inProgress.remove_if([] (Objective *o) { return o->m_mission->isMainMission() == false; });
	m_inProgressActive.remove_if([] (Objective *o) { return o->m_mission->isMainMission() == false; });
	m_missionsActive.remove_if([] (Mission *m) { return m->isMainMission() == false; });
	for (Mission *mission : m_missions) {
		if (mission->isMainMission() == false)
			mission->reloadIfInProgress();
	}
}

void ObjectiveManager::deactivateAll() {
	for (Mission *mission : m_missions)
		mission->reload();
	m_missionsActive.clear();
	m_inProgress.clear();
	m_inProgressActive.clear();
}

void ObjectiveManager::uninit() {
	for (Mission *mission : m_missions) {
		for (Objective *objective : mission->m_primaryObjectives)
			delete objective;
		mission->m_primaryObjectives.clear();

		for (Objective *objective : mission->m_secondaryObjectives)
			delete objective;
		mission->m_secondaryObjectives.clear();

		delete mission;
	}
	m_missions.clear();
	m_inProgress.clear();
	m_inProgressActive.clear();
	m_missionsActive.clear();
}

Mission *ObjectiveManager::getMission(uint32_t dtpIndex) {
	for (Mission *mission : m_missions)
		if (mission->m_dtpIndex == dtpIndex)
			return mission;

	Mission *mission = new Mission(this, dtpIndex);
	m_missions.push_front(mission);
	return mission;
}

void ObjectiveManager::recomputeActiveObjectives() {
	m_inProgressActive.clear();
	for (Mission *activeMission : m_missionsActive)
		for (Objective *objective : m_inProgress)
			if (objective->m_mission == activeMission)
				m_inProgressActive.push_front(objective);
}

void ObjectiveManager::moveMissionToFront(Mission *mission) {
	for (auto it=m_missions.begin(); it != m_missions.end(); it++)
		if (*it == mission) {
			m_missions.erase(it);
			break;
		}
	m_missions.push_front(mission);
}

Objective *ObjectiveManager::getObjective(uint32_t dtpIndex) {
	for (Mission *mission : m_missions) {
		for (Objective *objective : mission->m_primaryObjectives)
			if (objective->m_dtpIndex == dtpIndex)
				return objective;
		for (Objective *objective : mission->m_secondaryObjectives)
			if (objective->m_dtpIndex == dtpIndex)
				return objective;
	}
	return nullptr;
}

ObjectiveState ObjectiveManager::getObjectiveState(uint32_t dtpIndex) {
	if (auto *objective = getObjective(dtpIndex))
		return objective->m_state;
	return ObjectiveState::kNotAssigned;
}

void ObjectiveManager::enterInProgress(Objective *objective) {
	m_inProgress.push_front(objective);
	recomputeActiveObjectives();
}

void ObjectiveManager::leaveInProgress(Objective *objective) {
	for (auto it=m_inProgress.begin(); it != m_inProgress.end(); it++)
		if (*it == objective) {
			m_inProgress.erase(it);
			break;
		}
	recomputeActiveObjectives();
}

void ObjectiveManager::serialize(BinaryWriter&) { /*TODO*/ }
void ObjectiveManager::deserialize(BinaryReader&) { /*TODO*/ }
// static void ObjectiveManager::message143(void *msgData) { /*TODO*/ }
