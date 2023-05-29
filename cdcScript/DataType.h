#pragma once
#include <cstdint>

namespace cdc {

class ScriptType;
class DataType { // 20
public:

	enum BaseType { // 26
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
		SCRIPT = 11, // GCObject
		STRUCT = 12,
		STRUCT_REF = 13, // RCObject
		ENUM = 14,
		NTENUM = 15
	};

	struct Compound { // 203
		ScriptType *m_script;
		DataType *m_subType;
	};

	uint8_t type;
	uint8_t count;
	uint8_t n2;
	uint8_t n3;
	uint16_t field4;
	uint16_t padding6;
	void *ptr8;
};

}
