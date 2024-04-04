#pragma once
#include "cdcObjects/UberObject.h"

struct GameTracker;

void UBEROBJECT_DX3_Init(Instance*, GameTracker*);

class UberObjectComposite_DX3 : public UberObjectComposite {
public:
	UberObjectComposite_DX3(Instance *instance);

	// inherit method4
	// inherit method8

	// virtual void methodC();
	virtual UberObjectSection *createSection(Instance *instance, dtp::UberObjectProp::SectionProp *info, uint32_t index); // 10
	// virtual void method14();
	// virtual void method18();
	// virtual void method1C();
};

class UberObjectSection_DX3 : public UberObjectSection {
public:
	UberObjectSection_DX3(Instance *instance, UberObjectComposite_DX3 *composite, dtp::UberObjectProp::SectionProp *info, uint32_t index);

	// void method4() override;
	// void method8() override;

	// inherit methodC

	// void method10() override;
	// void method14() override;
	// void method18() override;
	// void method1C() override;
	// void method20() override;
	// void method24() override;

	// inherit method28
	// inherit method2C

	// virtual void method30();
};
