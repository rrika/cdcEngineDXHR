#include <cstdio>
#include "DRM.h"
#include "Resolve.h"
#include "ResolveObject.h"
#include "ResolveSection.h"

namespace cdc {

void Resolve::Create() {
	// TODO
	ResolveSection::Create();
	// TODO
}

void Resolve::Destroy() {
	// TODO
	ResolveSection::Destroy();
	// TODO
}

int32_t Resolve::Release(ResolveObject*& pObject) {
	// TODO: interact with StreamDeferredManager
	if (pObject != nullptr)
		if (auto ref = pObject->Release())
			return pObject = nullptr, ref;

	return 0;
}

int32_t Resolve::GetRefCount(ResolveObject *pObject) {
	return pObject ? pObject->m_ref : -1;
}

void SectionRecord::Release(ResolveObject *pObject) { // 331
	for (uint32_t i=0; i<m_numEntries; i++)
		ReleaseEntry(i, pObject);
	delete[] m_pEntry;
	delete this;
}

void SectionRecord::ReleaseEntry(uint32_t index, ResolveObject *pObject) { // 330
	Entry& entry = m_pEntry[index];
	if (entry.contentType == 1) // empty section
		return;
	if (entry.domainID != 0xffffffff) {
		printf("releasing type %d id %d (section %x)\n",
			entry.contentType, entry.domainID, entry.sectionID);
		g_resolveSections[entry.contentType]->ReleaseResource(entry.domainID);
	}
}

}
