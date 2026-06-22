#include "AugmentationSystem.h"
#include "game/dtp/objecttypes/globalplayerinfo.h"

extern GlobalPlayerInfo *globalPlayerInfo;

AugmentationSystem::AugmentationSystem() {
	upgrades[kEnergyRegenDelay] = new UpgradeDescriptor_float {"EnergyRegenDelay", 5.0};
	upgrades[kEnergyPointsPerCell] = new UpgradeDescriptor_float {"EnergyPointsPerCell", 50.0};
	upgrades[kEnergyNumOfCells] = new UpgradeDescriptor_uint32 {"EnergyNumOfCells", 2};
	upgrades[kEnergyRechargeRate] = new UpgradeDescriptor_float {"EnergyRechargeRate", 3.0};
	upgrades[kJumpLevel] = new UpgradeDescriptor_uint32 {"JumpLevel", 0};
	upgrades[kEnableAugmentedSprint] = new UpgradeDescriptor_bool8 {"EnableAugmentedSprint", false};
	upgrades[kEnableBungee] = new UpgradeDescriptor_bool8 {"EnableBungee", false};
	upgrades[kBungeeEnergyCost] = new UpgradeDescriptor_float {"BungeeEnergyCost", 24.0};
	upgrades[kEnablePunchThroughWall] = new UpgradeDescriptor_bool8 {"EnablePunchThroughWall", false};
	upgrades[kPunchThroughWallEnergyCost] = new UpgradeDescriptor_float {"PunchThroughWallEnergyCost", 20.0};
	upgrades[kTakeDownNumTargets] = new UpgradeDescriptor_uint32 {"TakeDownNumTargets", 0};
	upgrades[kTakeDownSingleKillEnergyCost] = new UpgradeDescriptor_float {"TakeDownSingleKillEnergyCost", 25.0};
	upgrades[kTakeDownSingleStunEnergyCost] = new UpgradeDescriptor_float {"TakeDownSingleStunEnergyCost", 15.0};
	upgrades[kTakeDownMultiKillEnergyCost] = new UpgradeDescriptor_float {"TakeDownMultiKillEnergyCost", 50.0};
	upgrades[kTakeDownMultiStunEnergyCost] = new UpgradeDescriptor_float {"TakeDownMultiStunEnergyCost", 30.0};
	upgrades[kFiringRecoil] = new UpgradeDescriptor_float {"FiringRecoil", 1.0};
	upgrades[kInventoryWidth] = new UpgradeDescriptor_uint32 {"InventoryWidth", 0};
	upgrades[kInventoryHeight] = new UpgradeDescriptor_uint32 {"InventoryHeight", 0};
	upgrades[kPrimaryDamageTaken] = new UpgradeDescriptor_float {"PrimaryDamageTaken", 1.0};
	upgrades[kEnableEMPShielding] = new UpgradeDescriptor_bool8 {"EnableEMPShielding", false};
	upgrades[kClaymoreDamage] = new UpgradeDescriptor_float {"ClaymoreDamage", 0.0};
	upgrades[kClaymoreEnergyCost] = new UpgradeDescriptor_float {"ClaymoreEnergyCost", 15.0};
	upgrades[kEnableUseDeviceRemotely] = new UpgradeDescriptor_bool8 {"EnableUseDeviceRemotely", false};
	upgrades[kHackingCPUCycles] = new UpgradeDescriptor_uint32 {"HackingCPUCycles", 0};
	upgrades[kHackingCaptureSoftwareRating] = new UpgradeDescriptor_uint32 {"HackingCaptureSoftwareRating", 1};
	upgrades[kHackingFortifySoftwareRating] = new UpgradeDescriptor_uint32 {"HackingFortifySoftwareRating", 0};
	upgrades[kHackingStealthSoftwareRating] = new UpgradeDescriptor_uint32 {"HackingStealthSoftwareRating", 0};
	upgrades[kHackingDetectionFeedback] = new UpgradeDescriptor_bool8 {"HackingDetectionFeedback", false};
	upgrades[kHackingAPIsAnalyzeLevel] = new UpgradeDescriptor_uint32 {"HackingAPIsAnalyzeLevel", 0};
	upgrades[kHackingDatastoresAnalyzeLevel] = new UpgradeDescriptor_uint32 {"HackingDatastoresAnalyzeLevel", 0};
	upgrades[kEnableHackCameras] = new UpgradeDescriptor_bool8 {"EnableHackCameras", false};
	upgrades[kEnableHackTurrets] = new UpgradeDescriptor_bool8 {"EnableHackTurrets", false};
	upgrades[kEnableHackRobots] = new UpgradeDescriptor_bool8 {"EnableHackRobots", false};
	upgrades[kEnableConversationPersuasionLevel] = new UpgradeDescriptor_bool8 {"EnableConversationPersuasionLevel", false};
	upgrades[kEnableConversationNPCAttributes] = new UpgradeDescriptor_bool8 {"EnableConversationNPCAttributes", false};
	upgrades[kEnableConversationClues] = new UpgradeDescriptor_bool8 {"EnableConversationClues", false};
	upgrades[kEnableConversationSpotNPCs] = new UpgradeDescriptor_bool8 {"EnableConversationSpotNPCs", false};
	upgrades[kEnableConversationSpecialAttacks] = new UpgradeDescriptor_bool8 {"EnableConversationSpecialAttacks", false};
	upgrades[kEnableStealthNoiseFeedback] = new UpgradeDescriptor_bool8 {"EnableStealthNoiseFeedback", false};
	upgrades[kEnableStealthVisionFeedback] = new UpgradeDescriptor_bool8 {"EnableStealthVisionFeedback", false};
	upgrades[kReticleGrowth] = new UpgradeDescriptor_float {"ReticleGrowth", 1.0};
	upgrades[kRadarRange] = new UpgradeDescriptor_float {"RadarRange", -1.0};
	upgrades[kRadarUnseenEnemies] = new UpgradeDescriptor_bool8 {"RadarUnseenEnemies", false};
	upgrades[kImmunedToGasAndHazards] = new UpgradeDescriptor_bool8 {"ImmunedToGasAndHazards", false};
	upgrades[kSprintDuration] = new UpgradeDescriptor_float {"SprintDuration", 0.0};
	upgrades[kSprintMaxCooldown] = new UpgradeDescriptor_float {"SprintMaxCooldown", 5.0};
	upgrades[kEnableHeavyObjectsGrab] = new UpgradeDescriptor_bool8 {"EnableHeavyObjectsGrab", false};
	upgrades[kGrabLightObjectEnergyCost] = new UpgradeDescriptor_float {"GrabLightObjectEnergyCost", 0.0};
	upgrades[kGrabHeavyObjectEnergyCost] = new UpgradeDescriptor_float {"GrabHeavyObjectEnergyCost", 5.0};
	upgrades[kThrowLightObjectEnergyCost] = new UpgradeDescriptor_float {"ThrowLightObjectEnergyCost", 5.0};
	upgrades[kThrowHeavyObjectEnergyCost] = new UpgradeDescriptor_float {"ThrowHeavyObjectEnergyCost", 10.0};
	upgrades[kEnableXRayVision] = new UpgradeDescriptor_bool8 {"EnableXRayVision", false};
	upgrades[kXRayEnergyCostOverTime] = new UpgradeDescriptor_float {"XRayEnergyCostOverTime", 1.0};
	upgrades[kRunSilently] = new UpgradeDescriptor_bool8 {"RunSilently", false};
	upgrades[kSprintSilently] = new UpgradeDescriptor_bool8 {"SprintSilently", false};
	upgrades[kJumpLandSilently] = new UpgradeDescriptor_bool8 {"JumpLandSilently", false};
	upgrades[kSilentRunEnergyCostOverTime] = new UpgradeDescriptor_float {"SilentRunEnergyCostOverTime", 1.0};
	upgrades[kEnableCamouflage] = new UpgradeDescriptor_bool8 {"EnableCamouflage", false};
	upgrades[kCamouflageEnergyCostOverTime] = new UpgradeDescriptor_float {"CamouflageEnergyCostOverTime", 5.0};
	upgrades[kAlarmTimer] = new UpgradeDescriptor_bool8 {"AlarmTimer", false};
	upgrades[kFlashSuppresant] = new UpgradeDescriptor_bool8 {"FlashSuppressant", false};
	upgrades[kWirelessInteractionMaxRange] = new UpgradeDescriptor_float {"WirelessInteractionMaxRange", 1500.0};
	upgrades[kSyntheticPheromonesPropagator] = new UpgradeDescriptor_bool8 {"SyntheticPheromonesPropagator", false};
	upgrades[kMarksNumber] = new UpgradeDescriptor_uint32 {"MarksNumber", 0};
	upgrades[kLastKnownPosition] = new UpgradeDescriptor_bool8 {"LastKnownPosition", false};

	UpgradeInfo *info = globalPlayerInfo->upgradeInfo;
	for (auto i=0u; i<info->numInitialValues; i++) {
		auto& entry = info->initialValues[i];
		upgrades[entry.upgradeType]->setValue((void*)&entry.data);
	}
}

AugmentationSystem::~AugmentationSystem() {
	for (auto i=0u; i<kUpgradeCount; i++)
		delete upgrades[i];
}
