#include <span>
#include "Intro.h"
#include "stream.h"
#include "StreamUnit.h"

/*
class IntroDataTable {
	using ProcessFunction = void();
	struct IntroDataTableEntry {
		int32_t m_introDataType;
		ProcessFunction *m_introDataProcessFunction;
	}

public:
	void IntroDataTable();
	void ProcessIntroData(Instance*);
	void RegisterIntroDataType(int32_t dataType, ProcessFunction *function);
};
*/

namespace IntroManager {

dtp::Intro *FindIntro(cdc::Level *level, int32_t introUniqueID) { // line 148
	dtp::ADMD *admd = level->admdData;
	for (auto& intro : std::span(admd->objects, admd->numObjects))
		if (intro.uniqueID == introUniqueID)
			return &intro;
	// TODO: scan markers as well
	return nullptr;
}

dtp::Intro *FindIntroUniqueID(int32_t introUniqueID, int32_t *streamUnitID) { // line 188
	if (streamUnitID)
		*streamUnitID = 0;

	for (auto &unit : StreamTracker) {
		if (unit.used /*== 2*/ && unit.level) {
			dtp::Intro *intro = FindIntro(unit.level, introUniqueID);
			if (intro) {
				if (streamUnitID)
					*streamUnitID = unit.level->streamUnitID;
				return intro;
			}
		}
	}

	return nullptr;
}

}
