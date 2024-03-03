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

