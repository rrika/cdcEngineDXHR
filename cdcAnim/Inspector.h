#pragma once

namespace cdc {

class AnimComponentV2;
struct AnimFragment;

}

struct UIActions;

void buildUI(UIActions& uiact, cdc::AnimFragment*);
void buildUI(UIActions& uiact, cdc::AnimComponentV2*);
