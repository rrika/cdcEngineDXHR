#pragma once
#include "cdcObjects/UberObject.h"

struct GameTracker;

void UBEROBJECT_DX3_Init(Instance*, GameTracker*);

class UberObjectComposite_DX3 : public UberObjectComposite {
public:
	UberObjectComposite_DX3(Instance *instance);
};

class UberObjectSection_DX3 : public UberObjectSection {

};
