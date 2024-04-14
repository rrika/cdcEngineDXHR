#pragma once
#include <cstdint>
#include "cdcObjects/Objects.h"
#include "cdc/dtp/objecttypes/uberobject.h"

// There is a string .\game\Objects\UberObject.cpp in DXHRDC.exe suggesting this file should go there,
// but Tomb Raider later moved it to cdcObjects/ which makes more sense IMO.

class Instance;
class UberObjectSection;

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
	bool checkCondition(dtp::UberObjectProp::QueryProp&);

	virtual ~UberObjectComposite() = default;
	virtual void commandByName(uint32_t); // 4
	virtual void commandByIndex(int); // 8
	virtual bool methodC(dtp::UberObjectProp::Unknown& entry, int, void*);
	virtual UberObjectSection *createSection(Instance *instance, dtp::UberObjectProp::SectionProp *info, uint32_t index); // 10
	// virtual void method14();
	// virtual void write(BinaryWriter&); // 18
	// virtual void read(BinaryReader&); // 1C
};

class UberObjectSection : public ObjState {
	uint32_t magic = 0xF0012345; // 50

public:
	UberObjectSection(Instance *instance, UberObjectComposite *composite, dtp::UberObjectProp::SectionProp *info, uint32_t index);

	virtual ~UberObjectSection() = default;
	virtual void method4(dtp::UberObjectProp::Consequence& conseq);
	// virtual void method8();
	// virtual void methodC();
	// virtual void method10();
	// virtual void method14();
	// virtual void method18();
	// virtual void write(BinaryWriter&); // 1C
	// virtual void read(BinaryReader&); // 20
	// virtual void method24();
	// virtual void method28();
	// virtual void method2C();
};
