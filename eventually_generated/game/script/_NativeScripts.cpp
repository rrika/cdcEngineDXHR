#include "cdcScript/cdcScript.h"
#include "cdcScript/NativeScriptType.h"
#include "cdcScript/ScriptManager.h"
#include "cdcScript/ScriptObject.h"
#include "cdcScript/ScriptSymbol.h"
#include "game/_NsScaleformMovieController.h"
#include "game/script/game/NsScaleformMovieController.h"

using namespace cdc;

int32_t _NsScaleformMovieController::s_virtual = 0;

class NsScaleformMovieController_ScriptType : public NativeScriptType {
public:
	NsScaleformMovieController_ScriptType();
	bool InitType(ScriptType*) override;

	static void _ProcessMovieFunction(void *instance, int32_t numArgs, DataValue *args, void *retVal);
};

NsScaleformMovieController_ScriptType::NsScaleformMovieController_ScriptType() : // line 32520
	NativeScriptType("game/script/game/game", "scaleformmoviecontroller")
{
} 

bool NsScaleformMovieController_ScriptType::InitType(ScriptType *type) { // line 32579

	NativeScriptType::InitType(type);
	InitFunction(0, &_ProcessMovieFunction);
	// InitFunction(1, &_ProcessMovieFunction);
	// InitFunction(2, &_ProcessMovieFunction);
	// TODO

	_NsScaleformMovieController::s_virtual = type->blob->prototypes[0].vtIndex;

	return true;
}

void NsScaleformMovieController_ScriptType::_ProcessMovieFunction( // line 32624
	void *instance, int32_t numArgs, DataValue *args, void *retVal)
{
	auto *controller = (NsScaleformMovieController*) instance;
	*(uint32_t*)retVal = controller->ProcessMovieFunction(
		*(ScriptSymbol*)
			&args[0],
		*(cdc::ScriptNativeDynArray<NtScaleformValue>*)
			args[1].address
	);
}

int32_t _NsScaleformMovieController::ProcessMovieFunction( // line 32685
	ScriptSymbol& methodName,
	ScriptNativeDynArray<NtScaleformValue> const& args)
{
	int32_t retval;
	uint8_t state = m_self->GetState();
	
	DataValue funArgs[2] = {
		{ .stringValue = methodName.m_string },
		{ .address = (uint8_t*)&args }
	};

	auto *func = m_self->GetFunction(state, _NsScaleformMovieController::s_virtual);
	m_self->CallFunction(func, 2, funArgs, &retval);
	return retval;
}

void Init_NativeScripts() { // line 44434
	// TODO
	auto *scriptManager = cdc::ScriptManager::s_instance;
	// scriptManager->RegisterNativeScriptType(new NsSmartFunction_Actor_Action_Request_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsSmartFunction_Actor_Movement_Teleport_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsSmartBool_Actor_Action_IsPerforming_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsSmartBool_Actor_AI_IsAlarmPanelDisabled_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsSmartBool_Actor_AI_IsThreatRightOfCover_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsSmartBool_Actor_AiState_Is_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsSmartBool_Actor_Health_IsDead_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsSmartBool_Actor_IsPlayer_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsSmartBool_Actor_Weapon_IsWielding_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsSmartEuler_Actor_Aiming_GetAbsoluteAnglesToTarget_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsSmartEuler_Actor_Aiming_GetAbsoluteAngularVelocity_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsSmartFloat_Actor_Health_Get_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsSmartReference_Actor_GetPlayer_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsSmartReference_Actor_Inventory_PrimaryWeapon_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NtAnchoredCamera_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NtAutoSplineCamera_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NtCameraAnchor_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NtCameraRef_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NtCameraSpline_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NtTransitionCamera_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NtClock_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsCoreBase_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NtMessageChannel_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NtPosition_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NtRotation_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NtSymbol_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NtVector_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NtAccessCode_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NtAction_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsActor_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsActorNpc_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsActorPlayer_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NtActorSensor_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NtActorTuneData_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsAiController_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NtAiFaction_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NtAiGroup_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NtAIManager_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsAimingController_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsAimingController_AbsolutePosition_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsAimingController_Keyframes_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsAimingController_Player_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsAimingController_Player_NamirControl_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsAimingController_TargetedActor_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NtAiState_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NtAiZone_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NtAnchor_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NtAnimated_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NtAnimation_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NtAnimGraphRTValue_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NtAnimGraphTransition_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NtArchetype_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NtAtom_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NtAugmentationUpgrade_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NtBarkEvent_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NtBinaryReader_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NtBinaryWriter_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NtBlackboard_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NtBlackboardData_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NtBriefing_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NtCell_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NtCinematic_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NtCinematicEvent_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsCinematicHelper_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsCinematicManager_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NtClass_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsController_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NtConversation_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NtConversationHistory_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NtConversationValue_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NtDamageType_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsDeadBodyBase_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NtDefendPoint_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NtDeferredLight_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NtDeferredObjAnimation_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NtDeferredObjGroup_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NtDFTime_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsDispatcherBase_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NtDisturbance_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsDLCSystem_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NtDynamicFragment_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NtEffect_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NtExperienceReward_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsGameBase_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NtGameFlag_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsGameGlobalsBase_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NtGlobalEffect_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NtGlobalParams_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsGlobalScriptBase_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NtGlobalsRef_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NtGoal_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NtGOAPScriptSymbol_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsGripController_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsGripFederovaController_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsGripNamirClimbController_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsGripNamirController_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NtHealth_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsHeightController_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsHeightController_Combat_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsHeightController_Cover_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NtHostedAnimStateGraph_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NtHudGroup_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NtInputMap_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsInstance_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NtInstanceRef_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NtInstanceStatus_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NtInventoryItem_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NtJoint_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsJumpControllerNamir_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NtJumpHelper_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NtLight_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NtLimit_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsLocomotionController_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsLocomotionController_Keyframes_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsLocomotionController_Player_Cover_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsLocomotionController_Player_RelativeToActor_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NtLookAt_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NtLookAtController_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsMapMarkerData_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NtMarker_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NtMarkup_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NtMarkupRef_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NtMicrophone_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NtMission_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NtModelMarker_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NtModifiable_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NtMotor_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NtMultibody_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NtMusic_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsNamirWhip_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NtNPCInteraction_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NtObjective_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NtObjectiveScriptableLocator_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NtObjectRef_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NtOccupation_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsOLBase_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NtOLDelete_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NtOLFeed_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NtOLFileEntry_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NtOLFriend_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NtOLMsg_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NtPadshock_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsPhysicsObjectKillVolume_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NtPickup_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsPickupBase_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NtPlan_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NtPlannerObstacle_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NtPlannerVolume_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NtPlayer_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsPlayerController_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NtPlayerRef_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NtPortal_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsProjectile_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsQuest_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NtRadarManager_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NtRangeSensor_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NtRestrictedArea_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NtReverb_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NtReverbPreset_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsRigidBodyBreakableBase_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NtScaleformDynamicTexture_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NtScaleformFunction_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NtScaleformMovie_ScriptType());
	scriptManager->RegisterNativeScriptType(new NsScaleformMovieController_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NtScaleformValue_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsScenario_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsScenarioManager_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NtScenarioRef_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsScriptedSequence_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NtSequencerGraph_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsSmartAngle_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsSmartAngle_ApproachTarget_Duration_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsSmartAngle_LazyEvaluation_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsSmartBase_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsSmartBool_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsSmartBool_FromActor_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsSmartBool_LeftHanded_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsSmartBool_Not_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsSmartBool_SignToggled_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NtSmartContext_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsSmartEuler_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsSmartEuler_BoneRotation_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsSmartEuler_FromActor_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsSmartEuler_FromInstance_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsSmartEuler_InstanceRotation_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsSmartEuler_LazyEvaluation_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsSmartEuler_PlayerCamera_Rotation_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsSmartFloat_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsSmartFloat_AdaptiveHeight_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsSmartFloat_Add_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsSmartFloat_AngleFromAimingToCover_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsSmartFloat_ApproachTarget_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsSmartFloat_ApproachTarget_AccelerationTable_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsSmartFloat_ApproachTarget_Damping_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsSmartFloat_ApproachTarget_MassSpringDamper_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsSmartFloat_ApproachTarget_ScaledDamping_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsSmartFloat_ApproachTarget_SimpleMovingAverage_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsSmartFloat_ApproachTarget_Transition_Duration_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsSmartFloat_ApproachTarget_Transition_Proportional_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsSmartFloat_CallFunction_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsSmartFloat_Constraint_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsSmartFloat_Constraint_Deceleration_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsSmartFloat_FromActor_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsSmartFloat_HeadToward_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsSmartFloat_InputAxis_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsSmartFloat_LazyEvaluation_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsSmartFloat_Math_CubicCurve_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsSmartFloat_SampledEvaluation_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsSmartFloat_WeaponSpinUpRatio_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsSmartFloat_Yaw_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsSmartFunction_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsSmartFunction_Actor_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsSmartFunction_CallFunction_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsSmartFunction_CameraShake_Play_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsSmartFunction_CameraShake_Start_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsSmartFunction_Instance_SetVisibility_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsSmartFunction_PadShock_Play_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsSmartFunction_PadShock_Start_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsSmartFunction_Sound_Play_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsSmartFunction_Sound_Start_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsSmartInt_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsSmartInt_FromActor_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsSmartReference_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsSmartReference_CallFunction_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NtSmartScript_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsSmartString_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsSmartVector_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsSmartVector_BonePosition_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsSmartVector_FindGround_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsSmartVector_FromInstance_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsSmartVector_InstancePosition_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsSmartVector_LazyEvaluation_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsSmartVector_MarkerPosition_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsSmartVector_MarkupPosition_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsSmartVector_PlayerCamera_Position_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NtSound_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NtSoundEndType_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NtSoundGroup_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NtSoundSpline_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NtSpatialVolume_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NtSpawnPoint_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsSquibBase_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NtStance_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NtStickyBool_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NtStickyFloat_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NtStickyInt_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NtStimulus_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsTakeDownScriptedSequence_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NtTechTree_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NtTerrainGroup_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NtTimeManager_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsTriggerPlaneBase_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsTriggerVolumeBase_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NtTrophy_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NtTutorial_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NtTutorialData_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NtTvShow_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsUberObjectBase_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NtUberObjectCommand_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NtUberObjectEvent_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NtUberObjectJoint_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NtUberObjectQuery_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NtUberObjectSectionID_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsUnit_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NtUnitRef_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NtUpgradeDescriptor_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NtValue_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NtVideo_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NtVO_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsVolume_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NtWaypointNode_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NtWaypointSet_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsWeapon_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsPs3MainMenuMoviecontroller_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsAlarmPanelMovieController_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsAugmentationWheelMovieController_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsBombMovieController_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsBombTimerCinematicMovieController_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsBrieferMovieController_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsCinematicCommentaryOverlayMovieController_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsCinematicPauseMenuMovieController_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsCodeCollectorMovieController_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsConfigMovieController_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsContextualActionsMovieController_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsContextualLegendsMovieController_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsConversationChoiceMovieController_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsDamageArrowsMovieController_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsDeathMenuMovieController_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsDebugMenuMovieController_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsDedicatedTerminalMovieController_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsDefectiveHUDMovieController_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsDLCPopupMovieController_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsE3GameIntroMovieController_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsEmailsMovieController_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsEndingMovieController_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsEnergyBarMovieController_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsGameIntroMovieController_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsGameMapMovieController_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsGameMenuMovieController_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsGlitchMovieController_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsGrenadesIndicatorMovieController_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsHackingMovieController_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsHealthBarMovieController_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsInfoCastMovieController_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsInventoryMovieController_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsIReaderMovieController_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsKeypadMovieController_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsLoadingMovieController_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsLoginMovieController_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsLogUpdatesMovieController_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsLootSelectorMovieController_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsMainMenuMovieController_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsMakingOfMovieController_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsMapSelectorMovieController_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsMediaLogMovieController_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsMenuWheelMovieController_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsMissionLogMovieController_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsNewsReaderMovieController_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsNPCMarkingSystemMovieController_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsObjectivesLocatorMovieController_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsOnGunWeaponIndicatorMovieController_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsParallaxMovieController_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsPauseMenuMovieController_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsPersonalityTypeMovieController_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsPopupManagerMovieController_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsQuickBarMovieController_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsQuickSelectorMovieController_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsRadarMovieController_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsReticleMovieController_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsRootMovieController_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsSaveLoadMovieController_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsScaleformPromptController_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsSecurityHubMovieController_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsSelectionBoxMinimalMovieController_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsSelectionBoxMovieController_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsShopMovieController_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsSocialAugmentationMovieController_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsSprintBarMovieController_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsStealthEnhancerMovieController_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsSubtitlesMovieController_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsTargetLockingMovieController_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsTechTreeMovieController_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsTutorialMovieController_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsTutorialViewerMovieController_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsUberObjectMovieController_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsVideoCinematicPauseMenuMovieController_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsVideoMovieController_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsWeaponHeatingMovieController_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsWeaponIndicatorMovieController_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsWeaponInfoMovieController_ScriptType());
	// scriptManager->RegisterNativeScriptType(new NsWorldSpaceSelectionBoxMovieController_ScriptType());
}
