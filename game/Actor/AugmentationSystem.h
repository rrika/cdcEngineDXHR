#pragma once
#include <cstdint>
// #include <vector>

struct UpgradeDescriptorBase {
	char const *m_name;
public:
	UpgradeDescriptorBase(const char *name) : m_name(name) {}

	virtual uint32_t getUint32() const { return 0; }
	virtual bool getBool() const { return false; }
	virtual float getFloat() const { return 0.0f;}
	virtual void resetPtr() = 0;
	virtual void setValue(void*) = 0;
	virtual void setPtr(void*) = 0;
	virtual void setPtr2(void*) = 0;
	virtual ~UpgradeDescriptorBase() = default; // HACK

	const char *getName() const { return m_name; }
};

struct UpgradeDescriptor_uint32 : public UpgradeDescriptorBase {
	uint32_t m_uint;
	uint32_t *m_pData = &m_uint;
	uint32_t *m_pData2 = 0;
public:
	UpgradeDescriptor_uint32(const char *name, uint32_t value) : UpgradeDescriptorBase(name), m_uint(value) {}
	uint32_t getUint32() const { return m_pData2 ? *m_pData2 : *m_pData; }
	void resetPtr() { m_pData = &m_uint; }
	void setValue(void *p) { m_uint = *(uint32_t*)p; }
	void setPtr(void *p) { m_pData = (uint32_t*)p; }
	void setPtr2(void *p) {  m_pData2 = (uint32_t*)p; }
};

struct UpgradeDescriptor_bool8 : public UpgradeDescriptorBase {
	bool m_bool;
	bool *m_pData = &m_bool;
	bool *m_pData2 = 0;
public:
	UpgradeDescriptor_bool8(const char *name, bool value) : UpgradeDescriptorBase(name), m_bool(value) {}
	bool getBool() const { return m_pData2 ? *m_pData2 : *m_pData; }
	void resetPtr() { m_pData = &m_bool; }
	void setValue(void *p) { m_bool = *(bool*)p; }
	void setPtr(void *p) { m_pData = (bool*)p; }
	void setPtr2(void *p) {  m_pData2 = (bool*)p; }
};

struct UpgradeDescriptor_float : public UpgradeDescriptorBase {
	float m_float;
	float *m_pData = &m_float;
	float *m_pData2 = 0;
public:
	UpgradeDescriptor_float(const char *name, float value) : UpgradeDescriptorBase(name), m_float(value) {}
	float getFloat() const { return m_pData2 ? *m_pData2 : *m_pData; }
	void resetPtr() { m_pData = &m_float; }
	void setValue(void *p) { m_float = *(float*)p; }
	void setPtr(void *p) { m_pData = (float*)p; }
	void setPtr2(void *p) {  m_pData2 = (float*)p; }
};

enum Upgrade {
	kEnergyRegenDelay = 0,
	kEnergyPointsPerCell = 1,
	kEnergyNumOfCells = 2,
	kEnergyRechargeRate = 3,
	kJumpLevel = 4,
	kEnableAugmentedSprint = 5,
	kEnableBungee = 6,
	kBungeeEnergyCost = 7,
	kEnablePunchThroughWall = 8,
	kPunchThroughWallEnergyCost = 9,
	kTakeDownNumTargets = 10,
	kTakeDownSingleKillEnergyCost = 11,
	kTakeDownSingleStunEnergyCost = 12,
	kTakeDownMultiKillEnergyCost = 13,
	kTakeDownMultiStunEnergyCost = 14,
	kFiringRecoil = 15,
	kInventoryWidth = 16,
	kInventoryHeight = 17,
	kPrimaryDamageTaken = 18,
	kEnableEMPShielding = 19,
	kClaymoreDamage = 20,
	kClaymoreEnergyCost = 21,
	kEnableUseDeviceRemotely = 22,
	kHackingCPUCycles = 23,
	kHackingCaptureSoftwareRating = 24,
	kHackingFortifySoftwareRating = 25,
	kHackingStealthSoftwareRating = 26,
	kHackingDetectionFeedback = 27,
	kHackingAPIsAnalyzeLevel = 28,
	kHackingDatastoresAnalyzeLevel = 29,
	kEnableHackCameras = 30,
	kEnableHackTurrets = 31,
	kEnableHackRobots = 32,
	kEnableConversationPersuasionLevel = 33,
	kEnableConversationNPCAttributes = 34,
	kEnableConversationClues = 35,
	kEnableConversationSpotNPCs = 36,
	kEnableConversationSpecialAttacks = 37,
	kEnableStealthNoiseFeedback = 38,
	kEnableStealthVisionFeedback = 39,
	kReticleGrowth = 40,
	kRadarRange = 41,
	kRadarUnseenEnemies = 42,
	kImmunedToGasAndHazards = 43,
	kSprintDuration = 44,
	kSprintMaxCooldown = 45,
	kEnableHeavyObjectsGrab = 46,
	kGrabLightObjectEnergyCost = 47,
	kGrabHeavyObjectEnergyCost = 48,
	kThrowLightObjectEnergyCost = 49,
	kThrowHeavyObjectEnergyCost = 50,
	kEnableXRayVision = 51,
	kXRayEnergyCostOverTime = 52,
	kRunSilently = 53,
	kSprintSilently = 54,
	kJumpLandSilently = 55,
	kSilentRunEnergyCostOverTime = 56,
	kEnableCamouflage = 57,
	kCamouflageEnergyCostOverTime = 58,
	kAlarmTimer = 59,
	kFlashSuppresant = 60,
	kWirelessInteractionMaxRange = 61,
	kSyntheticPheromonesPropagator = 62,
	kMarksNumber = 63,
	kLastKnownPosition = 64,
	kUpgradeCount = 65
};

struct AugmentationSystem /* : BaseSystem */ {
	// uint32_t experiencePoints; // 8
	// uint32_t praxisPoints; // 10
	// uint32_t dword14;
	// float float18;
	// uint8_t byte1C;
	// std::vector<uint32_t> augDtp20;
	// std::vector<uint32_t> augDtp2C;
	// uint32_t dword38;
	// uint32_t dword3C;
	UpgradeDescriptorBase *upgrades[kUpgradeCount]; // 40
	// uint8_t byte144;
public:
	AugmentationSystem();
	~AugmentationSystem();
	bool hasUpgrade(Upgrade u) const { return upgrades[u]->getBool(); }
	uint32_t getUint(Upgrade u) const { return upgrades[u]->getUint32(); }
};
