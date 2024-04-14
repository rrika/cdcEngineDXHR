#include "UberObject_DX3.h"

void UBEROBJECT_DX3_Init(Instance *instance, GameTracker*) {
	// TODO
	new UberObjectComposite_DX3(instance);
}

UberObjectComposite_DX3::UberObjectComposite_DX3(Instance *instance) :
	UberObjectComposite(instance, /*deferSectionInit=*/ true)
{
	CreateSections(instance);
}

UberObjectSection *UberObjectComposite_DX3::createSection(Instance *instance, dtp::UberObjectProp::SectionProp *info, uint32_t index) {
	return new UberObjectSection_DX3(instance, this, info, index);
}

UberObjectSection_DX3::UberObjectSection_DX3(Instance *instance, UberObjectComposite_DX3 *composite, dtp::UberObjectProp::SectionProp *info, uint32_t index) :
	UberObjectSection(instance, composite, info, index)
{
	// TODO
}

void UberObjectSection_DX3::method4(dtp::UberObjectProp::Consequence& conseq) {
	// TODO
	switch (conseq.kind) {
	case 5:
		// TODO
	case 6:
		// TODO
	case 7:
		// TODO
	case 8:
		// TODO
	case 9:
		// TODO
	case 10:
		// TODO
	default:
		UberObjectSection::method4(conseq);
		break;
	}
}
