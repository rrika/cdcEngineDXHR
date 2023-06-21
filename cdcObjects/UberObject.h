#pragma once
#include <cstdint>
#include "cdcObjects/Objects.h"
#include "cdc/dtp/objecttypes/uberobject.h"

// There is a string .\game\Objects\UberObject.cpp in DXHRDC.exe suggesting this file should go there,
// but Tomb Raider later moved it to cdcObjects/ which makes more sense IMO.

class Instance;

Instance *UBEROBJECT_BirthSectionInstance(Instance *parent, uint32_t modelIndex, uint32_t id);

class UberObjectComposite : public ObjState {
	Instance *instance; // 54
	bool createdSections = false; // 5D
	uint32_t sectionsIntroUniqueID = ~0u; // 60
	int32_t *sectionList = nullptr; // 64
	uint32_t numSections = 0; // 68
public:
	UberObjectComposite(Instance *instance, bool deferSectionInit);
	void CreateSections(Instance *instance);
	Instance *CreateSectionInstance(Instance *instance, dtp::UberObjectProp::SectionProp *info, uint32_t index);
};

class UberObjectSection : public ObjState {
public:
	UberObjectSection(Instance *instance, UberObjectComposite *composite, dtp::UberObjectProp::SectionProp *info, uint32_t index);
};
