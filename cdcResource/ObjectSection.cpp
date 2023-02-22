#include <cstdio>
#include <cstring>
#include "ObjectSection.h"
#include "ResolveObject.h"
#include "cdcFile/FileHelpers.h"
#include "cdcFile/FileSystem.h"
#include "cdcObjects/Object.h"
#include "cdcObjects/ObjectManager.h"
#include "config.h"

#ifdef ENABLE_IMGUI
#include "imgui/imgui.h"
#endif

namespace cdc {

uint32_t ObjectSection::realize(uint32_t sectionId, uint32_t unknown6, uint32_t size, bool& alreadyLoaded) {
	auto entry = getByObjectListIndex(sectionId);
	if (!entry) {
		entry = allocObjectSlot(sectionId, 2);

	} else if (entry->state != 1) {
		entry->refCount++;
		alreadyLoaded = true;
		return entry - objects;

	} else {
		entry->state = 2;
	}

	entry->objBlob = (Object*) new char[size];
	return entry - objects;
}

void ObjectSection::fill(uint32_t domainId, void* src, size_t size, size_t offset) {
	auto &entry = objects[domainId];
	memcpy(entry.objBlob + offset, src, size);
	// return size;
}

void ObjectSection::construct(uint32_t domainId, void *) {
	ObjectTracker *objectTracker = &objects[domainId];
	Object *objBlob = objectTracker->objBlob;
	printf("object\n");
	printf("  domainId      = %x\n", domainId);
	printf("  objectTracker = %p\n", objectTracker);
	printf("  objBlob       = %p\n", objBlob);
	printf("  dtpData       = %p\n", objBlob->dtpData);
	if (objBlob->dtpData) {
		// TODO
		printf("  numModels     = %d\n", objBlob->dtpData->numModels);
		objBlob->numModels = objBlob->dtpData->numModels;
		// TODO
		printf("  models        = %p\n", objBlob->dtpData->models);
		objBlob->models = objBlob->dtpData->models;
		if (objBlob->models && objBlob->numModels != 0)
			printf("  models[0]     = %p\n", objBlob->models[0]);

		// TODO		
		printf("  TuneDataPtr   = %p\n", objBlob->dtpData->TuneDataPtr);
		if (auto TuneDataPtr = objBlob->dtpData->TuneDataPtr) {
			objBlob->data = TuneDataPtr;
			objBlob->dtpDataCopy = objBlob->dtpData;
		}
	}
}

void* ObjectSection::getWrapped(uint32_t domainId) {
	return objects[domainId].objBlob;
}

void* ObjectSection::getBlob(uint32_t domainId) {
	return objects[domainId].objBlob;
}

uint32_t ObjectSection::getDomainId(uint32_t sectionId) {
	for (uint32_t i = 0; i < 604; i++)
		if (objects[i].state && objects[i].objectListIndex == sectionId)
			return i;
	return ~0u;
}

}
