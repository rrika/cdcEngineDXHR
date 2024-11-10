#pragma once
#include <cstdint>

namespace cdc {

class ScriptType;
class NativeScriptType;
struct StructDecl; // ScriptType.h

class DataType { // 20
public:

	enum BaseType { // line 26
		DTVOID = 0,
		BOOL = 1,
		INT = 2,
		FLOAT = 3,
		STRING = 4,
		STATE = 5,
		ARRAY = 6,
		DYNARRAY = 7,
		MAP = 8,
		UNKNOWN = 9,
		NATIVE = 10,
		SCRIPT = 11,
		STRUCT = 12,
		STRUCT_REF = 13,
		ENUM = 14,
		NTENUM = 15
	};

	struct Compound { // line 203
		ScriptType *m_script;
		DataType *m_subType;
	};

	uint8_t type;
	uint8_t count;
	uint8_t n2;
	uint8_t n3;
	uint16_t field4;
	uint16_t padding6;
	union { // 8
		DataType *m_subType;
		// EnumDecl *m_enum;
		StructDecl *m_struct;
		ScriptType *m_script;
		DataType::Compound *m_compound;
	};

	NativeScriptType* GetNativeType();
	ScriptType* GetScriptType();
};

}
