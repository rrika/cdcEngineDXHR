#include "config.h"
#include "cdcScript/Decompiler.h"
#include "cdcScript/ScriptType.h"
#include "UIActions.h"

#if ENABLE_IMGUI
#include "imgui/imgui.h"
#endif

using namespace cdc;

static const char* nameof(ScriptType *ty) {
	if (!ty)
		return "(null)";
	else if (ty->blob->name)
		return ty->blob->name;
	else
		return "Unknown";
}

void Decompile(UIActions& uiact, ScriptType& ty) {
#if ENABLE_IMGUI
	ImGui::Text("class %s {", nameof(&ty));
	ImGui::Text("   // TODO");
	ImGui::Text("}");
#endif
}
