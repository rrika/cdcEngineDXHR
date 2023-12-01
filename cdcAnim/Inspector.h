#pragma once

namespace cdc {

class AnimComponentV2;
struct AnimFragment;

}

struct UIActions;

void animTrackUI(bool fmt16, void *offsetTable, uint16_t *lengthTable);
void buildUI(UIActions& uiact, cdc::AnimFragment*);
void buildUI(UIActions& uiact, cdc::AnimComponentV2*);
