#include "DRMIndex.h"

DRMIndex drmIndex;

bool locate(void *ptr, cdc::DRMSectionHeader *&header, ptrdiff_t &offset) {
	auto addr = uintptr_t(ptr);
	auto it = drmIndex.loadedSections.upper_bound(addr-1);
	if (it == drmIndex.loadedSections.end())
		return false;

	auto begin = it->first;
	auto &loaded = it->second;
	if (addr - begin >= loaded.size)
		return false;

	header = &drmIndex.sectionHeaders[*loaded.drmPath.get()][loaded.sectionIndex];
	offset = (addr-begin);
	return true;
}
