#pragma once

namespace dtp {
	struct Intro;
}

struct DeferredRenderingExtraData;
struct LensFlareAndCoronaExtraData;
struct UIActions;

void buildUI(DeferredRenderingExtraData *extraData);
void buildUI(LensFlareAndCoronaExtraData *extraData);
void buildUI(UIActions& uiact, dtp::Intro *intro);
