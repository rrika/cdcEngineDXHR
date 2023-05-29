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

#if ENABLE_IMGUI
static void Type(UIActions& uiact, DataType *dt) {
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
		case DataType::STRUCT:   simple = "struct(TODO)"; break;
		case DataType::STRUCT_REF: simple = "structref(TODO)"; break;
		case DataType::ENUM:     simple = "enum(TODO)"; break;
		case DataType::NTENUM:   simple = "ntenum(TODO)"; break;

		case DataType::NATIVE:
		case DataType::SCRIPT:
		{
			ImGui::PushID((void*)dt);
			if (dt->ptr8 == nullptr)
				ImGui::Text("(null)");
			else if (dt->type & 0x80) {
				// A<B>
				auto *cty = (DataType::Compound*)dt->ptr8;
				auto *ty = cty->m_script;
				auto *sdt = cty->m_subType;
				if (ImGui::SmallButton(nameof(ty)))
					uiact.select(ty);
				ImGui::SameLine();
				ImGui::Text("<");
				ImGui::SameLine();
				Type(uiact, sdt);
				ImGui::SameLine();
				ImGui::Text(">");
			} else {
				// A
				auto *ty = (ScriptType*)dt->ptr8;
				if (ImGui::SmallButton(nameof(ty)))
					uiact.select(ty);
			}
			ImGui::PopID();
			return;
		}
	}
	ImGui::Text("%s", simple);
}
#endif

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
	if (ty.blob->members) {
		uint32_t numMembers = ((uint32_t*)ty.blob->members)[-1];
		for (uint32_t i=0; i<numMembers; i++) {
			DataMember *m = &ty.blob->members[i];
			Type(uiact, &m->type);
			ImGui::SameLine();
			ImGui::Text("field_%x;", m->offset);
		}
	}
	if (ty.blob->functions) {
		uint32_t numFunctions = ((uint32_t*)ty.blob->functions)[-1];
		for (uint32_t i=0; i<numFunctions; i++) {
			Function *fn = &ty.blob->functions[i];
			Prototype *pt = fn->prototype;
			Type(uiact, &pt->returnType); ImGui::SameLine();
			ImGui::Text("method_%d(", i);
			ImGui::SameLine();
			uint32_t numArgs = pt->GetNumArgs();
			for (uint32_t j=0; j<numArgs; j++) {
				if (j) {
					ImGui::Text(",");
					ImGui::SameLine();
				}
				DataMember *m = &pt->args[i];
				Type(uiact, &m->type);
				ImGui::SameLine();
				ImGui::Text("arg_%x", m->offset);
				ImGui::SameLine();
			}
			ImGui::SameLine();
			ImGui::Text(");");
		}
	} else {
		ImGui::Text("// no functions");
	}
	ImGui::Unindent();
	ImGui::Text("}");
#endif
}
