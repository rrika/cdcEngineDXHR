#include "cdc/dtp/soundplex.h"
#include "SoundPlex.h"
#include "SoundPlexChoiceList.h"

namespace cdc {

SoundPlex *SoundPlexChoiceList::GetPlayableSoundFromChoiceList(
	dtp::SoundPlex::ChoiceList& choiceList,
	SoundTypes::Controls *pControls,
	SoundTypes::Controls3d *pControls3d,
	SoundOwner *pOwner)
{
	if (choiceList.m_numSounds == 0)
		return nullptr;

	if (choiceList.m_numSounds == 1)
		return SoundPlexCollection::Create(choiceList.m_sounds[0], pControls, pControls3d, pOwner);

	// TODO: implement modes other than cycle
	uint32_t index = choiceList.m_counter++;
	if (choiceList.m_counter == choiceList.m_numSounds)
		choiceList.m_counter = 0;

	return SoundPlexCollection::Create(choiceList.m_sounds[index], pControls, pControls3d, pOwner);
}

}
