#pragma once
#include "cdcScript/cdcScript.h" // for DataValue
#include "cdcScript/DataType.h"
#include "cdcScript/ScriptName.h"
#include "cdcSys/RCObject.h"
#include "cdcSys/SArray.h"

namespace cdc {

class NativeScriptType;
class ScriptObject;
class ScriptType;

struct DataMember { // line 219
	DataType m_type; // 0
	uint16_t m_offset; // C
	uint16_t m_name; // E
	uint32_t m_init; // 10
};

struct Prototype { // 377
	ScriptType *scriptType;
	uint8_t flags4;
	uint8_t callType; // 5
	uint16_t vtIndex; // 6
	uint16_t id8;
	uint16_t idA;
	DataMember *args;
	DataType returnType;

	uint32_t GetNumArgs() {
		return args ? ((uint32_t*)args)[-1] : 0;
	}
};

class Function { // 477
public:
	enum Flags {
		NATIVE = 1
	};

	Prototype *prototype;
	uint8_t  flags; // 4
	uint32_t padding8;
	uint32_t paddingC;
	uint32_t padding10;
	CallbackCallFunction *nativeFunc; // 14
	SArray<uint32_t> m_scriptFunc;
};

static_assert(sizeof(Function) == 0x1C);

struct VTable { // 594
	uint16_t size;
	Function *funcs[];
};

struct VTableArray { // 624
	uint16_t size;
	uint16_t field_2;
	uint16_t field_4;
	uint16_t field_6;
	VTable **table;
};

struct ScriptTypeStreamData { // 692
	uint32_t    m_version;
	uint16_t    field_4;
	uint16_t    field_6;
	ScriptName  m_name; // 08
	char       *m_nativeScriptPackageName; // 0C
	char       *m_nativeScriptName; // 10
	ScriptType *m_scriptType; // 14
	ScriptType *m_superScriptType; // 18
	// TODO
	uint8_t     padding[0x2C-0x1C];
	// TODO
	SArray<DataMember>  m_members; // 2C
	uint32_t            padding30;
	Prototype          *m_prototypes; // 34
	SArray<Function>    m_functions; // 38
	VTableArray         m_vtables; // 3C
	SArray<ScriptType*> scriptTypeImports; // 48
	uint16_t m_depth; // 4C
	uint16_t m_packageName; // 4E
};

static_assert(sizeof(ScriptTypeStreamData) == 0x50);

class ScriptType : public RCObject { // 749
public:
	ScriptTypeStreamData* blob;
	NativeScriptType* nativeScriptType = nullptr;
public:
	ScriptObject *CreateObject();

	ScriptType(uint32_t size) {
		blob = (ScriptTypeStreamData*) new char[size];
	}
	Function *GetVFunction(int32_t state, int32_t vtIndex) {
		VTable *table = blob->m_vtables.table[vtIndex];
		if (state >= table->size)
			return table->funcs[0];
		else
			return table->funcs[state];
	}
	virtual ~ScriptType() {
		// TODO
		delete[] (char*)blob;
	}
	virtual void finalize() { /*TODO*/ }

	ScriptType *getParentType() { return blob->m_superScriptType; }

	ScriptType *getClosestNativeAncestor(); // HACK
};

}
