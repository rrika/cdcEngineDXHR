#pragma once
#include "cdcSys/SArray.h"
#include "cdcSys/HashSet.h"
#include "ScriptType.h"

namespace cdc {

class ScriptDataCursor {
protected:
	ScriptType        *scriptType;
	SArray<DataMember> pDataMemberArray;
	DataMember        *cursor;
	DataMember        *end;
	void refill();

public:
	ScriptDataCursor(ScriptType *scriptType);
	bool done() const { return cursor == end; }
	DataMember *operator->() const { return cursor; }
	operator DataMember*() const { return cursor; }
	virtual ~ScriptDataCursor() = default;
	virtual void advance();
};

struct ScriptUniqueDataCursor : public ScriptDataCursor {
	HashSet<uint16_t> seen;
	uint32_t field20;
	uint32_t field24;
public:
	ScriptUniqueDataCursor(ScriptType *scriptType);
	void advance() override;
};

}
