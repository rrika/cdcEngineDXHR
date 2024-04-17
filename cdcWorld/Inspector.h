#pragma once

namespace dtp {
	struct Intro;
}

struct DeferredRenderingExtraData;
struct LensFlareAndCoronaExtraData;
struct UIActions;
class Instance;

void buildUI(UIActions& uiact, DeferredRenderingExtraData *extraData);
void buildUI(UIActions& uiact, LensFlareAndCoronaExtraData *extraData);
void buildUI(UIActions& uiact, dtp::Intro *intro, Instance *instance=nullptr);
void buildUI(UIActions& uiact, Instance *instance);
