#include "cdcFile/FileHelpers.h"
#include "cdcFile/FileSystem.h"
#include "cdcLocale/localstr.h"
#include "Specialisation.h"

using namespace cdc;

uint32_t Specialisation::s_targetMask;
uint32_t Specialisation::s_phase = 2;

void Specialisation::BlockingChange(uint32_t newMask) {
	StartChange(newMask);
	while (s_phase != 2)
		ContinueChange();
}

void Specialisation::StartChange(uint32_t newMask) {
	FileSystem *fs = getDefaultFileSystem();
	if (fs->getLanguageMask() != newMask) {
		s_phase = 0;
		s_targetMask = newMask;
	}
}

void Specialisation::ContinueChange() {
	if (s_phase == 2)
		return;

	getDefaultFileSystem()->processRequest();
	if (getDefaultFileSystem()->hasRequests())
		return;
	s_phase++;

	if (s_phase == 1) {
		uint32_t oldMask = getDefaultFileSystem()->getLanguageMask();
		getDefaultFileSystem()->setLanguageMask(s_targetMask);
		localstr_reload();
		// TODO: Resolve::ChangeSpecialisation(oldMask, s_targetMask);
	}
}
