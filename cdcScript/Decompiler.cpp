#include "config.h"
#include "cdcObjects/ObjectManager.h"
#include "cdcResource/DRMIndex.h"
#include "cdcResource/DTPDataSection.h"
#include "cdcScript/DataType.h"
#include "cdcScript/Decompiler.h"
#include "cdcScript/ScriptDataCursor.h"
#include "cdcScript/ScriptType.h"
#include "cdcWorld/InstanceManager.h"
#include "game/DX3Player.h"
#include "game/ObjectiveManager.h"
#include "UIActions.h"

#if ENABLE_IMGUI
#include "imgui/imgui.h"
#endif

using namespace cdc;

static const char* nameof(ScriptType *ty) {
	if (!ty)
		return "(null)";
	else if (ty->blob->m_nativeScriptName)
		return ty->blob->m_nativeScriptName;
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
			if (dt->m_script == nullptr)
				ImGui::Text("(null)");
			else if (dt->type & 0x80) {
				// A<B>
				auto *cty = dt->m_compound;
				auto *ty = cty->m_script;
				auto *sdt = cty->m_subType;
				if (ImGui::SmallButton(nameof(ty)))
					uiact.select(ty);
				ImGui::SameLine(0.f, 0.f);
				ImGui::Text("<");
				ImGui::SameLine(0.f, 0.f);
				Type(uiact, sdt);
				ImGui::SameLine(0.f, 0.f);
				ImGui::Text(">");
			} else {
				// A
				auto *ty = dt->m_script;
				if (ImGui::SmallButton(nameof(ty)))
					uiact.select(ty);
			}
			ImGui::PopID();
			return;
		}
	}
	ImGui::Text("%s", simple);
}

static void InitNative(UIActions& uiact, ScriptType *ty, void *init_) {
	ty = ty->getClosestNativeAncestor();
	if (!ty) {
		ImGui::Text("[ERROR: not a native type]");
		return;
	}

	const char *ntyname = ty->blob->m_nativeScriptName;

	ObjectiveManager& objectiveManager = *PlayerPair::s_pair->getPlayer0()->objectiveManager;

	if (strcmp(ntyname, "objective") == 0) {
		struct ObjectiveInit { uint32_t objectiveId; };
		auto *init = *(ObjectiveInit**)init_;
		ImGui::Text("objective %d/0x%x", init->objectiveId, init->objectiveId);
		if (Objective *o = objectiveManager.getObjective(init->objectiveId)) {
			ImGui::SameLine(0.f, 0.f);
			ImGui::TextDisabled(" /* %s */", o->getName());
		}

	} else if (strcmp(ntyname, "mission") == 0) {
		struct MissionInit { uint32_t missionId; };
		auto *init = *(MissionInit**)init_;
		ImGui::Text("mission %d/%x", init->missionId, init->missionId);
		if (Mission *m = objectiveManager.getMission(init->missionId)) {
			ImGui::SameLine(0.f, 0.f);
			ImGui::TextDisabled(" /* %s */", m->getName());
		}

	} else if (strcmp(ntyname, "scenarioref") == 0) {
		struct ScenarioRef { uint32_t scenarioId; };
		auto *init = *(ScenarioRef**)init_;
		ImGui::Text("scenario %d/0x%x", init->scenarioId, init->scenarioId);

	} else if (strcmp(ntyname, "instanceref") == 0) {
		struct InstanceRefInit { uint32_t id; };
		auto *init = *(InstanceRefInit**)init_;
		ImGui::Text("instance %d/0x%x", init->id, init->id);
		auto& map = InstanceManager::gInstanceIntroUniqueIDHashMap;
		if (auto it = map.find(init->id); it != map.end()) {
			ImGui::SameLine();
			if (ImGui::SmallButton("inspect")) {
				uiact.select(it->second);
			}
		}

	} else if (strcmp(ntyname, "uberobjectevent") == 0 || strcmp(ntyname, "uberobjectcommand") == 0) {
		struct UberObjectEventOrCommandInit { uint16_t objListIndex; uint32_t index; };
		auto *init = *(UberObjectEventOrCommandInit**)init_;
		ImGui::Text("{object %s, %x}", objectName(init->objListIndex), init->index);

	} else if (
		strcmp(ntyname, "stickybool") == 0 ||
		strcmp(ntyname, "stickyfloat") == 0 ||
		strcmp(ntyname, "stickyint") == 0
	) {
		struct StickyInit { uint16_t varName; };
		auto *init = *(StickyInit**)init_;
		ImGui::Text("{ 0x%x }", init->varName);

	} else {
		ImGui::Text("todo(%s)", ntyname);
	}
}

static void Init(UIActions& uiact, DataMember *member) {
	if (!member)
		return;

	ImGui::SameLine();

	uint32_t init = member->m_init;
	switch (member->m_type.type & 15) {
	case DataType::DTVOID:
		ImGui::Text("= (void)0");
		break;

	case DataType::BOOL:
		ImGui::TextUnformatted(bool(init & 0xff) ? "= true" : "= false");
		break;

	case DataType::INT:
		ImGui::Text("= %d", init);
		break;

	case DataType::FLOAT: {
		float f; memcpy(&f, &init, 4);
		ImGui::Text("= %f", f);
		break;
	}
	case DataType::STRING:
		ImGui::Text("= \"%s\"", (const char*)init);
		break;

	case DataType::STATE:
		ImGui::Text("= state %d", init);
		break;

	case DataType::ARRAY:
	case DataType::DYNARRAY:
	case DataType::MAP:
	case DataType::UNKNOWN:
		goto todo;

	case DataType::NATIVE:
		if (init) {
			ImGui::Text("=");
			ImGui::SameLine();
			InitNative(uiact, member->m_type.GetScriptType(), (void*)init);
		}
		break;

	case DataType::SCRIPT:
	case DataType::STRUCT:
	case DataType::STRUCT_REF:
	case DataType::ENUM:
	case DataType::NTENUM:
		goto todo;

	todo:
		ImGui::Text("= todo(%d)", member->m_type.type & 15);
		break;
	}
}

#endif

void Decompile(UIActions& uiact, ScriptType& ty) {
#if ENABLE_IMGUI
	{
		const char *path = nullptr;
		cdc::DRMSectionHeader *header = nullptr;
		ptrdiff_t offset;
		if (locate(&ty, header, offset))
			path = DTPDataSection::GetName(header->id);
		if (path)
			ImGui::TextDisabled("// %s", path);
	}
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
	if (false) {
		if (ty.blob->m_members) {
			uint32_t numMembers = ty.blob->m_members.size();
			for (uint32_t i=0; i<numMembers; i++) {
				DataMember *m = &ty.blob->m_members[i];
				Type(uiact, &m->m_type);
				ImGui::SameLine();
				ImGui::Text("field_%x", m->m_offset);
				Init(uiact, m);
				ImGui::SameLine(0.f, 0.f);
				ImGui::Text(";");
			}
		}
	} else {
		for (ScriptUniqueDataCursor c(&ty); !c.done(); c.advance()) {
			ImGui::PushID((DataMember*)c);
			Type(uiact, &c->m_type);
			ImGui::SameLine();
			ImGui::Text("field_%x", c->m_offset);	
			Init(uiact, c);
			ImGui::SameLine(0.f, 0.f);
			ImGui::Text(";");
			ImGui::PopID();
		}
	}
	if (ty.blob->m_functions) {
		uint32_t numFunctions = ty.blob->m_functions.size();
		for (uint32_t i=0; i<numFunctions; i++) {
			Function *fn = &ty.blob->m_functions[i];
			Prototype *pt = fn->prototype;
			Type(uiact, &pt->returnType);
			ImGui::SameLine();
			ImGui::Text("method_%d(", i);
			uint32_t numArgs = pt->GetNumArgs();
			for (uint32_t j=0; j<numArgs; j++) {
				if (j) {
					ImGui::SameLine(0.f, 0.f);
					ImGui::Text(",");
				}
				DataMember *m = &pt->args[i];
				ImGui::SameLine();
				Type(uiact, &m->m_type);
				ImGui::SameLine();
				ImGui::Text("arg_%x", m->m_offset);
			}
			ImGui::SameLine();
			ImGui::Text(");");
		}
	} else {
		ImGui::TextDisabled("// no functions");
	}
	ImGui::Unindent();
	ImGui::Text("}");
#endif
}
