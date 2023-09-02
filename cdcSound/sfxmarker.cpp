#include <cmath>
#include "cdcMath/Math.h"
#include "cdcMath/VectorInlines.h"
#include "sfxmarker.h"
#include "Microphone.h"
#include "SoundPlex.h"
#include "cdcWorld/stream.h"
#include "cdcWorld/StreamUnit.h"

namespace cdc {

bool neverInsideSfxPerimeter = false;

void SFXMARKER_ProcessAllMarkers() { // line 100
	for (uint32_t i=0; i<sizeof(StreamTracker)/sizeof(StreamUnit); i++) {
		StreamUnit& streamUnit = StreamTracker[i];
		// if (streamUnit.used != 2) continue;
		if (streamUnit.used == 0) continue;
		auto& admd = streamUnit.level->admdData;
		for (uint32_t j=0; j<admd->m_SfxMarkerCount; j++)
			SFXMARKER_Process(admd->m_ppSfxMarkers[j]);
	}
}

void TriggerAction(dtp::sfxmarker *marker, dtp::sfxmarker::Trigger *trigger) { // line 131
	auto& actionData = trigger->actionData;
	switch (trigger->action) {
	case dtp::sfxmarker::EnumAction_Start: SFXMARKER_Start(marker, actionData.Start); break; // 0
	case dtp::sfxmarker::EnumAction_End:   SFXMARKER_End(marker, actionData.End); break; // 1
	case dtp::sfxmarker::EnumAction_SetVariable: SFXMARKER_SetVariable(marker, actionData.SetVariable); break; // 2
	case dtp::sfxmarker::EnumAction_IncVariable: SFXMARKER_IncVariable(marker, actionData.IncVariable); break; // 3
	case dtp::sfxmarker::EnumAction_DecVariable: SFXMARKER_DecVariable(marker, actionData.DecVariable); break; // 4
	}
}

void SFXMARKER_Start(dtp::sfxmarker *marker, dtp::sfxmarker::unionStartList& list) { // line 159
	if (!marker->soundHandles) // allocated in SFXMARKER_Create
		return;

	for (uint32_t i=0; i<list.numSoundRefs; i++) {
		if (int32_t soundIndex = list.soundRefs[i]; soundIndex >= 0) {
			SoundHandle& sh = marker->soundHandles[soundIndex];
			// TODO: reference counting
			sh = SOUND_StartPaused(marker->soundData[soundIndex], 0.0f);
			sh->SetPosition(marker->position);
			sh->controls3d.playbackType = 0x101; // 3D
		}
	}
}

void SFXMARKER_End(dtp::sfxmarker *marker, dtp::sfxmarker::unionEndList& list) { // line 225
	for (uint32_t i=0; i<list.numSoundRefs; i++) {
		if (int32_t soundIndex = list.soundRefs[i]; soundIndex >= 0) {
			SoundHandle& sh = marker->soundHandles[soundIndex];
			// TODO: reference counting
			if (sh.isSet() && *sh) // HACK
				sh->End((SoundTypes::EndType)list.endType);
		}
	}
}

void SFXMARKER_SetVariable(dtp::sfxmarker *marker, dtp::sfxmarker::unionMusicVar& var) {
	// TODO
}

void SFXMARKER_IncVariable(dtp::sfxmarker *marker, dtp::sfxmarker::unionMusicVar& var) {
	// TODO
}

void SFXMARKER_DecVariable(dtp::sfxmarker *marker, dtp::sfxmarker::unionMusicVar& var) {
	// TODO
}

bool CheckPerimeter(dtp::sfxmarker::Perimeter *perimeter, dtp::sfxmarker *marker) { // line 264
	// TODO
	auto distanceVec = marker->position - Vector3{g_microphone.m_position};
	auto distanceSq = Dot3(distanceVec, distanceVec);
	auto distance = sqrtf(distanceSq);
	bool inside = perimeter->radius >= distance;
	if (neverInsideSfxPerimeter)
		inside = false; // override
	bool changed = perimeter->bBreached != inside;
	perimeter->bBreached = inside;
	perimeter->lastDistance = distance;
	return changed;
}

static void Increment(dtp::sfxmarker::Perimeter *perimeter) {
	perimeter->nCountFired++;
	if (perimeter->nCountFired == 0) // skip zero
		perimeter->nCountFired = 1;
}

void CheckTrigger(dtp::sfxmarker *marker, dtp::sfxmarker::Trigger *trigger) { // line 291
	auto perimeter = &trigger->conditionData;
	if (trigger->bEnabled) {
		if (trigger->condition != dtp::sfxmarker::EnumCondition_Init) {
			if ((perimeter->bAlways || perimeter->nCountFired) &&
				CheckPerimeter(perimeter, marker) &&
				perimeter->bBreached != (trigger->condition == dtp::sfxmarker::EnumCondition_Exit))
			{
				Increment(perimeter);
				TriggerAction(marker, trigger);
			}
		}
	} else {
		if (trigger->condition != dtp::sfxmarker::EnumCondition_Init) {
			trigger->bEnabled = true;
			CheckPerimeter(perimeter, marker);
			if (perimeter->bBreached && trigger->condition == dtp::sfxmarker::EnumCondition_Enter) {
				Increment(perimeter);
				TriggerAction(marker, trigger);
			}

		} else {
			trigger->bEnabled = true;
			TriggerAction(marker, trigger);
		}
	}
}

void SFXMARKER_Process(dtp::sfxmarker *marker) { // line 391
	for (uint32_t i=0; i<marker->numTriggers; i++)
		CheckTrigger(marker, &marker->triggers[i]);
}

void SFXMARKER_Create(dtp::sfxmarker *marker) { // line 418
	// TODO
	marker->soundHandles = new SoundHandle[marker->numSounds];
}

}
