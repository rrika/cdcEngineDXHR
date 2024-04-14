#pragma once
#include "cdcObjects/UberObject.h"

struct GameTracker;

void UBEROBJECT_DX3_Init(Instance*, GameTracker*);

class UberObjectComposite_DX3 : public UberObjectComposite {
public:
	UberObjectComposite_DX3(Instance *instance);

	// inherit commandByName
	// inherit commandByIndex

	// virtual void methodC();
	virtual UberObjectSection *createSection(Instance *instance, dtp::UberObjectProp::SectionProp *info, uint32_t index); // 10
	// virtual void method14();
	// virtual void write(BinaryWriter&); // 18
	// virtual void read(BinaryReader&); // 1C
};

class UberObjectSection_DX3 : public UberObjectSection {
public:
	UberObjectSection_DX3(Instance *instance, UberObjectComposite_DX3 *composite, dtp::UberObjectProp::SectionProp *info, uint32_t index);

	void method4(dtp::UberObjectProp::Consequence& conseq) override;
	// void method8() override;

	// inherit methodC

	// void method10() override;
	// void method14() override;
	// void method18() override;
	// void write(BinaryWriter&) override; // 1C
	// void read(BinaryReader&) override; // 20
	// void method24() override;

	// inherit method28
	// inherit method2C

	// virtual void method30();
};
