#include "config.h"
#include "cdcScript/DataType.h"
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

static void Type(DataType *dt) {
	if (!dt) {
		ImGui::Text("(null)");
		return;
	}
	const char *simple = nullptr;
	switch (dt->type & 15) {
		case DataType::DTVOID:   simple = "void"; break;
		case DataType::BOOL:     simple = "bool"; break;
		case DataType::INT:      simple = "int"; break;
		case DataType::FLOAT:    simple = "float"; break;
		case DataType::STRING:   simple = "string"; break;
		case DataType::STATE:    simple = "state"; break;
		case DataType::ARRAY:    simple = "array(TODO)"; break;
		case DataType::DYNARRAY: simple = "dynarray(TODO)"; break;
		case DataType::MAP:      simple = "map(TODO)"; break;
		case DataType::UNKNOWN:  simple = "unknown"; break;
		case DataType::NATIVE:   simple = "native(TODO)"; break;
		case DataType::SCRIPT:   simple = "script(TODO)"; break;
		case DataType::STRUCT:   simple = "struct(TODO)"; break;
		case DataType::STRUCT_REF: simple = "structref(TODO)"; break;
		case DataType::ENUM:     simple = "enum(TODO)"; break;
		case DataType::NTENUM:   simple = "ntenum(TODO)"; break;
	}
	ImGui::Text("%s", simple);
}

void Decompile(UIActions& uiact, ScriptType& ty) {
#if ENABLE_IMGUI
	ImGui::Text("class %s", nameof(&ty));
	ImGui::SameLine();
	if (auto parentTy = ty.getParentType()) {
		ImGui::Text(":");
		ImGui::SameLine();
		if (ImGui::SmallButton(nameof(parentTy))) {
			uiact.select(parentTy);
		}
		ImGui::SameLine();
	}
	ImGui::Text("{");
	ImGui::Indent();
	if (ty.blob->functions) {
		uint32_t numFunctions = ((uint32_t*)ty.blob->functions)[-1];
		ImGui::Text("// TODO: members");
		for (uint32_t i=0; i<numFunctions; i++) {
			Function *fn = &ty.blob->functions[i];
			Prototype *pt = fn->prototype;
			Type(&pt->returnType); ImGui::SameLine();
			ImGui::Text("method_%d(...);", i);
		}
	} else {
		ImGui::Text("// no functions");
	}
	ImGui::Unindent();
	ImGui::Text("}");
#endif
}
