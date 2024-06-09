#pragma once

class Instance;

namespace cdc {

void G2Instance_UpdateAllAnimComponents(float frameTime); // HACK
void G2Instance_BuildAllTransforms(); // guessed name
void G2Instance_BuildTransformsForList(Instance *instance);
bool G2Instance_NeedsBuildTransforms(Instance *instance);
void G2Instance_BuildTransforms(Instance *instance);
void G2Instance_RebuildTransforms(Instance *instance);
void G2Instance_PostRebuildTransforms(Instance *instance);
void G2Instance_SetTransformsToIdentity(Instance *instance);

}
