#include "ScriptDataCursor.h"

namespace cdc {

ScriptDataCursor::ScriptDataCursor(ScriptType *_scriptType) : scriptType(_scriptType) {
	pDataMemberArray = scriptType->blob->m_members;
	cursor = pDataMemberArray.begin();
	end    = pDataMemberArray.end();
	refill();
}

void ScriptDataCursor::refill() {
	if (scriptType == nullptr)
		return;

	while (cursor == end) {

		scriptType = scriptType->getParentType();
		if (scriptType == nullptr)
			break;

		auto blob = scriptType->blob;

		pDataMemberArray = scriptType->blob->m_members;
		cursor = pDataMemberArray.begin();
		end    = pDataMemberArray.end();
	}
}

void ScriptDataCursor::advance() {
	cursor++;
	refill();
}

ScriptUniqueDataCursor::ScriptUniqueDataCursor(ScriptType *scriptType) :
	ScriptDataCursor(scriptType),
	field20(0),
	field24(1)
{
	if (cursor < end)
		seen.insert(cursor->m_offset);
}

void ScriptUniqueDataCursor::advance() {
	do {
		ScriptDataCursor::advance();
	} while(cursor < end && seen.insert(cursor->m_offset).second == false);
}

}
