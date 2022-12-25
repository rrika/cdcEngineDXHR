#pragma once
#include "cdcSound.h"

namespace cdc {

class SoundPlexChoiceList : protected SoundTypes {
public:
	static SoundPlex *GetPlayableSoundFromChoiceList(
		dtp::SoundPlex::ChoiceList& choiceList,
		Controls *pControls,
		Controls3d *pControls3d,
		SoundOwner *pOwner);

	inline static SoundPlex *GetSoundPlexObject(
		dtp::SoundPlex::ChoiceList& choiceList,
		Controls *pControls,
		Controls3d *pControls3d,
		SoundOwner *pOwner)
	{
		return GetPlayableSoundFromChoiceList(choiceList, pControls, pControls3d, pOwner);
	}
};

}
