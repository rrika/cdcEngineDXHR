#pragma once
#include "cdc/dtp/objectproperties/sfxmarker.h"

namespace cdc {

void SFXMARKER_ProcessAllMarkers(); // line 100

void TriggerAction(dtp::sfxmarker *marker, dtp::sfxmarker::Trigger *trigger); // line 131
void SFXMARKER_Start(dtp::sfxmarker *marker, dtp::sfxmarker::unionStartList& list); // line 159
void SFXMARKER_End(dtp::sfxmarker *marker, dtp::sfxmarker::unionEndList& list); // line 225
void SFXMARKER_SetVariable(dtp::sfxmarker *marker, dtp::sfxmarker::unionMusicVar& var);
void SFXMARKER_IncVariable(dtp::sfxmarker *marker, dtp::sfxmarker::unionMusicVar& var);
void SFXMARKER_DecVariable(dtp::sfxmarker *marker, dtp::sfxmarker::unionMusicVar& var);

bool CheckPerimeter(dtp::sfxmarker::Perimeter& perimeter, dtp::sfxmarker *marker); // line 264
void CheckTrigger(dtp::sfxmarker *marker, dtp::sfxmarker::Trigger *trigger); // line 291
void SFXMARKER_Process(dtp::sfxmarker *marker); // line 391

void SFXMARKER_Create(dtp::sfxmarker *marker); // line 418

}
