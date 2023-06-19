#pragma once

namespace dtp {
	struct Intro;
}

struct DeferredRenderingExtraData;
struct LensFlareAndCoronaExtraData;
struct UIActions;
class Instance;

void buildUI(DeferredRenderingExtraData *extraData);
void buildUI(LensFlareAndCoronaExtraData *extraData);
void buildUI(UIActions& uiact, dtp::Intro *intro);
void buildUI(UIActions& uiact, Instance *instance);
