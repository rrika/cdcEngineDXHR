#pragma once
#include "cdcScript/cdcScript.h" // for DataValue
#include "cdcScript/DataType.h"
#include "cdcSys/RCObject.h"

namespace cdc {

class NativeScriptType;
class ScriptType;

struct DataMember { // 219
	DataType type;
	uint16_t offset;
	uint16_t name;
	uint32_t initializer;
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
	uint32_t padding18;
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
	uint32_t    scriptTypeVersion;
	uint16_t    field_4;
	uint16_t    field_6;
	uint16_t    id8;
	uint16_t    idA;
	char       *package; // 0C
	char       *name; // 10
	ScriptType *scriptTypeSelf; // 14
	ScriptType *scriptTypeParent; // 18
	// TODO
	uint8_t     padding[0x2C-0x1C];
	// TODO
	DataMember *members; // 2C
	uint32_t    padding30;
	Prototype  *prototypes; // 34
	Function   *functions; // 38
	VTableArray vtableArray; // 3C
};

static_assert(sizeof(ScriptTypeStreamData) == 0x48);

class ScriptType : public RCObject { // 749
public:
	ScriptTypeStreamData* blob;
	NativeScriptType* nativeScriptType;
public:
	ScriptType(uint32_t size) {
		blob = (ScriptTypeStreamData*) new char[size];
	}
	Function *GetVFunction(int32_t state, int32_t vtIndex) {
		VTable *table = blob->vtableArray.table[vtIndex];
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

	ScriptType *getParentType() { return blob->scriptTypeParent; }
};

}
