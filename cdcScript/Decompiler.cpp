#include "config.h"
#include "cdc/dtp/script/scriptsound.h"
#include "cdcObjects/ObjectManager.h"
#include "cdcResource/DRMIndex.h"
#include "cdcResource/DTPDataSection.h"
#include "cdcScript/ByteCodeEnum.h"
#include "cdcScript/DataType.h"
#include "cdcScript/Decompiler.h"
#include "cdcScript/ScriptDataCursor.h"
#include "cdcScript/ScriptType.h"
#include "cdcSound/SoundPlex.h"
#include "cdcWorld/InstanceManager.h"
#include "cdcWorld/stream.h"
#include "game/DX3Player.h"
#include "game/ObjectiveManager.h"
#include "game/dtp/objecttypes/globaldatabase.h"
#include "UIActions.h"

#if ENABLE_IMGUI
#include "imgui/imgui.h"
#endif

using namespace cdc;

extern GlobalDatabase *globalDatabase;

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

	} else if (strcmp(ntyname, "unitref") == 0) {
		struct UnitRefInit { uint16_t unitId; };
		auto *init = *(UnitRefInit**)init_;
		ImGui::Text("unit %s", unitNameById(init->unitId));

	} else if (strcmp(ntyname, "sound") == 0) {
		auto *init = *(dtp::ScriptSound**)init_;
		if (ImGui::SmallButton("Play")) {
			SOUND_StartPaused(&init->m_plex, 0.0f);
		}
		uiact.origin((void*)init);

	} else if (strcmp(ntyname, "action") == 0) {
		struct ActionDtp { const char *name; }; // in logicactionresource_database.drm
		struct ActionInit { ActionDtp *actionDtp; };
		auto *init = *(ActionInit**)init_;
		ImGui::Text("action %s", init->actionDtp ? init->actionDtp->name : nullptr);
		uiact.origin(init->actionDtp);

	} else if (strcmp(ntyname, "goal") == 0) {
		struct GoalInit { uint32_t goalIndex; };
		auto *init = *(GoalInit**)init_;
		if (globalDatabase) {
			auto& goal = globalDatabase->goals[init->goalIndex];
			ImGui::Text("goal %s", goal.name);
			uiact.origin(&goal);
		} else {
			ImGui::Text("goal %d", init->goalIndex);
		}

	} else if (strcmp(ntyname, "barkevent") == 0) {
		struct BarkEventInit { uint32_t barkEvent; };
		auto *init = *(BarkEventInit**)init_;
		if (globalDatabase) {
			ImGui::Text("barkevent %s", globalDatabase->barkEvents[init->barkEvent]);
		} else {
			ImGui::Text("barkevent %d", init->barkEvent);
		}

	} else {
		ImGui::Text("todo(%s)", ntyname);
		uiact.origin(init_);
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

static const char *opcodeNames[SCMD_NUM_BYTECODE] = {
	/* [SCMD_NOP] = */ "NOP", // 0x0
	/* [SCMD_POP] = */ "POP", // 0x1

	/* [SCMD_PUSH] = */ "PUSH", // 0x2
	/* [SCMD_PUSHI] = */ "PUSHI", // 0x3
	/* [SCMD_PUSHI8] = */ "PUSHI8", // 0x4
	/* [SCMD_PUSHC] = */ "PUSHC", // 0x5

	/* [SCMD_PUSHL8] = */ "PUSHL8", // 0x6
	/* [SCMD_PUSHL16] = */ "PUSHL16", // 0x7
	/* [SCMD_PUSHL32] = */ "PUSHL32", // 0x8
	/* [SCMD_PUSHLA] = */ "PUSHLA", // 0x9
	/* [SCMD_PUSHLR] = */ "PUSHLR", // 0xA

	/* [SCMD_PUSHS] = */ "PUSHS", // 0xB

	/* [SCMD_PUSHS8] = */ "PUSHS8", // 0xC
	/* [SCMD_PUSHS16] = */ "PUSHS16", // 0xD
	/* [SCMD_PUSHS32] = */ "PUSHS32", // 0xE
	/* [SCMD_PUSHSA] = */ "PUSHSA", // 0xF
	/* [SCMD_PUSHSR] = */ "PUSHSR", // 0x10

	/* [SCMD_PUSHM8] = */ "PUSHM8", // 0x11
	/* [SCMD_PUSHM16] = */ "PUSHM16", // 0x12
	/* [SCMD_PUSHM32] = */ "PUSHM32", // 0x13
	/* [SCMD_PUSHMA] = */ "PUSHMA", // 0x14
	/* [SCMD_PUSHMR] = */ "PUSHMR", // 0x15

	/* [SCMD_PUSHT8] = */ "PUSHT8", // 0x16
	/* [SCMD_PUSHT16] = */ "PUSHT16", // 0x17
	/* [SCMD_PUSHT32] = */ "PUSHT32", // 0x18
	/* [SCMD_PUSHTA] = */ "PUSHTA", // 0x19
	/* [SCMD_PUSHTR] = */ "PUSHTR", // 0x1A

	/* [SCMD_DREF8] = */ "DREF8", // 0x1B
	/* [SCMD_DREF16] = */ "DREF16", // 0x1C
	/* [SCMD_DREF32] = */ "DREF32", // 0x1D
	/* [SCMD_DREFA] = */ "DREFA", // 0x1E
	/* [SCMD_DREFR] = */ "DREFR", // 0x1F

	/* [SCMD_DREFD] = */ "DREFD", // 0x20

	/* [SCMD_BOUNDS] = */ "BOUNDS", // 0x21
	/* [SCMD_DBOUNDS] = */ "DBOUNDS", // 0x22

	/* [SCMD_MAP8] = */ "MAP8", // 0x23
	/* [SCMD_MAP16] = */ "MAP16", // 0x24
	/* [SCMD_MAP32] = */ "MAP32", // 0x25
	/* [SCMD_MAPA] = */ "MAPA", // 0x26
	/* [SCMD_MAPR] = */ "MAPR", // 0x27

	/* [SCMD_MAPG] = */ "MAPG", // 0x28

	/* [SCMD_IMAP8] = */ "IMAP8", // 0x29
	/* [SCMD_IMAP16] = */ "IMAP16", // 0x2A
	/* [SCMD_IMAP32] = */ "IMAP32", // 0x2B
	/* [SCMD_IMAPA] = */ "IMAPA", // 0x2C
	/* [SCMD_IMAPR] = */ "IMAPR", // 0x2D

	/* [SCMD_MOVE8] = */ "MOVE8", // 0x2E
	/* [SCMD_MOVE32] = */ "MOVE32", // 0x2F
	/* [SCMD_MOVER] = */ "MOVER", // 0x30
	/* [SCMD_MOVEC] = */ "MOVEC", // 0x31
	/* [SCMD_MOVEM] = */ "MOVEM", // 0x32
	/* [SCMD_MOVED] = */ "MOVED", // 0x33
	/* [SCMD_MOVEU] = */ "MOVEU", // 0x34
	/* [SCMD_MOVET] = */ "MOVET", // 0x35
	/* [SCMD_MOVEN] = */ "MOVEN", // 0x36
	/* [SCMD_MOVEAR] = */ "MOVEAR", // 0x37
	/* [SCMD_MOVEAC] = */ "MOVEAC", // 0x38
	/* [SCMD_MOVEAM] = */ "MOVEAM", // 0x39
	/* [SCMD_MOVEAD] = */ "MOVEAD", // 0x3A
	/* [SCMD_MOVEAU] = */ "MOVEAU", // 0x3B
	/* [SCMD_MOVEAT] = */ "MOVEAT", // 0x3C

	/* [SCMD_NEGI] = */ "NEGI", // 0x3D
	/* [SCMD_ADDI] = */ "ADDI", // 0x3E
	/* [SCMD_SUBI] = */ "SUBI", // 0x3F
	/* [SCMD_MULI] = */ "MULI", // 0x40
	/* [SCMD_DIVI] = */ "DIVI", // 0x41
	/* [SCMD_MODI] = */ "MODI", // 0x42
	/* [SCMD_LTI] = */ "LTI", // 0x43
	/* [SCMD_LTEQI] = */ "LTEQI", // 0x44
	/* [SCMD_EQI] = */ "EQI", // 0x45
	/* [SCMD_EQI8] = */ "EQI8", // 0x46

	/* [SCMD_NEGF] = */ "NEGF", // 0x47
	/* [SCMD_ADDF] = */ "ADDF", // 0x48
	/* [SCMD_SUBF] = */ "SUBF", // 0x49
	/* [SCMD_MULF] = */ "MULF", // 0x4A
	/* [SCMD_DIVF] = */ "DIVF", // 0x4B
	/* [SCMD_LTF] = */ "LTF", // 0x4C
	/* [SCMD_LTEQF] = */ "LTEQF", // 0x4D
	/* [SCMD_EQF] = */ "EQF", // 0x4E

	/* [SCMD_EQC] = */ "EQC", // 0x4F

	/* [SCMD_NOTB] = */ "NOTB", // 0x50
	/* [SCMD_ANDB] = */ "ANDB", // 0x51
	/* [SCMD_ORB] = */ "ORB", // 0x52
	/* [SCMD_XORB] = */ "XORB", // 0x53

	/* [SCMD_AND] = */ "AND", // 0x54
	/* [SCMD_OR] = */ "OR", // 0x55
	/* [SCMD_XOR] = */ "XOR", // 0x56
	/* [SCMD_SHL] = */ "SHL", // 0x57
	/* [SCMD_SHR] = */ "SHR", // 0x58
	/* [SCMD_INV] = */ "INV", // 0x59

	/* [SCMD_FTOI] = */ "FTOI", // 0x5A
	/* [SCMD_FTOB] = */ "FTOB", // 0x5B
	/* [SCMD_ITOF] = */ "ITOF", // 0x5C
	/* [SCMD_ITOB] = */ "ITOB", // 0x5D
	/* [SCMD_BTOI] = */ "BTOI", // 0x5E
	/* [SCMD_BTOF] = */ "BTOF", // 0x5F
	/* [SCMD_CTOI] = */ "CTOI", // 0x60
	/* [SCMD_CTOF] = */ "CTOF", // 0x61
	/* [SCMD_CTOB] = */ "CTOB", // 0x62

	/* [SCMD_JMP] = */ "JMP", // 0x63
	/* [SCMD_JMPF] = */ "JMPF", // 0x64

	/* [SCMD_CALLU] = */ "CALLU", // 0x65
	/* [SCMD_CALL1] = */ "CALL1", // 0x66
	/* [SCMD_CALL2] = */ "CALL2", // 0x67
	/* [SCMD_CALL3] = */ "CALL3", // 0x68
	/* [SCMD_CALL4] = */ "CALL4", // 0x69
	/* [SCMD_CALL5] = */ "CALL5", // 0x6A

	/* [SCMD_SSEQU] = */ "SSEQU", // 0x6B
	/* [SCMD_SSEQ1] = */ "SSEQ1", // 0x6C
	/* [SCMD_SSEQ2] = */ "SSEQ2", // 0x6D
	/* [SCMD_SSEQ3] = */ "SSEQ3", // 0x6E
	/* [SCMD_SSEQ4] = */ "SSEQ4", // 0x6F
	/* [SCMD_SSEQ5] = */ "SSEQ5", // 0x70

	/* [SCMD_PSEQU] = */ "PSEQU", // 0x71
	/* [SCMD_PSEQ1] = */ "PSEQ1", // 0x72
	/* [SCMD_PSEQ2] = */ "PSEQ2", // 0x73
	/* [SCMD_PSEQ3] = */ "PSEQ3", // 0x74
	/* [SCMD_PSEQ4] = */ "PSEQ4", // 0x75
	/* [SCMD_PSEQ5] = */ "PSEQ5", // 0x76

	/* [SCMD_START] = */ "START", // 0x77
	/* [SCMD_TSSTART] = */ "TSSTART", // 0x78
	/* [SCMD_TPSTART] = */ "TPSTART", // 0x79
	/* [SCMD_TEND] = */ "TEND", // 0x7A

	/* [SCMD_RET] = */ "RET", // 0x7B
	/* [SCMD_SLEEP] = */ "SLEEP", // 0x7C
	/* [SCMD_STATE] = */ "STATE", // 0x7D
	/* [SCMD_ISTYPE] = */ "ISTYPE", // 0x7E
	/* [SCMD_NEW] = */ "NEW", // 0x7F
	/* [SCMD_WAIT] = */ "WAIT", // 0x80
	/* [SCMD_PUSHD8] = */ "PUSHD8", // 0x81
	/* [SCMD_PUSHD16] = */ "PUSHD16", // 0x82
	/* [SCMD_PUSHD32] = */ "PUSHD32", // 0x83
	/* [SCMD_PUSHDA] = */ "PUSHDA", // 0x84
	/* [SCMD_MOVEP] = */ "MOVEP" // 0x85
};

void Signature(UIActions& uiact, uint32_t i, Function *fn);

static void ByteCode(UIActions& uiact, uint32_t *begin, uint32_t *end) {
	uint32_t *cursor = begin;
	while (cursor < end) {
		uint32_t op = (*cursor) >> 24;
		uint32_t zx = (*cursor) & 0xffffff;
		const char *name = op < SCMD_NUM_BYTECODE ? opcodeNames[op] : "INVALID";
		ImGui::TextDisabled("%04x: %08x %-7s", cursor-begin, *cursor, name);
		cursor++;

		switch (op) {
		case SCMD_PUSHC: // 0x5
		{
			const char *str = (const char*)*cursor++;
			ImGui::SameLine();
			ImGui::Text("\"%s\"", str);
			break;
		}
		case SCMD_PUSHL8: // 0x6
		case SCMD_PUSHL16: // 0x7
		case SCMD_PUSHL32: // 0x8
		case SCMD_PUSHLA: // 0x9
		case SCMD_PUSHLR: //0xA
			ImGui::SameLine();
			ImGui::Text("local_%x", zx);
			break;

		case SCMD_PUSHS8: // 0xC
		case SCMD_PUSHS16: // 0xD
		case SCMD_PUSHS32: // 0xE
		case SCMD_PUSHSA: // 0xF
		case SCMD_PUSHSR: //0x10
			ImGui::SameLine();
			ImGui::Text("field_%x", zx);
			break;

		case SCMD_CALLU: // 0x65
		case SCMD_CALL1: // 0x66
		case SCMD_CALL4: // 0x69
		{
			auto *func = (Function*)*cursor++;
			ImGui::SameLine();
			Signature(uiact, 9999, func);
			break;
		}

		case SCMD_CALL2: // 0x67
			cursor++;
			cursor++;
			break;

		case SCMD_CALL3: // 0x68
		case SCMD_CALL5: // 0x6A
			cursor++;
			break;

		case SCMD_ISTYPE: // 0x7E
		{
			auto *ty = (ScriptType*)*cursor++;
			ImGui::SameLine();
			ImGui::PushID(cursor);
			if (ImGui::SmallButton(nameof(ty)))
				uiact.select(ty);
			ImGui::PopID();
			break;
		}

		default: break;
		}
	}
}

void Signature(UIActions& uiact, uint32_t i, Function *fn) {
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
		DataMember *m = &pt->args[j];
		ImGui::SameLine();
		Type(uiact, &m->m_type);
		ImGui::SameLine();
		ImGui::Text("arg_%x", m->m_offset);
	}
	ImGui::SameLine();
	ImGui::Text(")");
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
			Signature(uiact, i, fn);
			ImGui::SameLine(0.f, 0.f);
			if (!fn->m_scriptFunc.m_ptr)
				ImGui::Text(";");
			else if (fn->m_scriptFunc.size() == 0) {
				ImGui::Text(" {");
				ImGui::SameLine();
				ImGui::TextDisabled("/* empty */");
				ImGui::SameLine();
				ImGui::Text("}");
			} else {
				ImGui::Text(" {");
				ImGui::Indent();
				for (auto& m : fn->m_locals) {
					Type(uiact, &m.m_type);
					ImGui::SameLine();
					ImGui::Text("local_%x", m.m_offset);
					Init(uiact, &m);
					ImGui::SameLine(0.f, 0.f);
					ImGui::Text(";");
				}
				ByteCode(uiact, fn->m_scriptFunc.begin(), fn->m_scriptFunc.end());
				ImGui::Unindent();
				ImGui::Text("}");
			}
		}
	} else {
		ImGui::TextDisabled("// no functions");
	}
	ImGui::Unindent();
	ImGui::Text("}");
#endif
}
