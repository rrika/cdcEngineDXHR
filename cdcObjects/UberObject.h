#pragma once
#include <cstdint>
#include "cdcObjects/Objects.h"
#include "cdc/dtp/objecttypes/uberobject.h"

// There is a string .\game\Objects\UberObject.cpp in DXHRDC.exe suggesting this file should go there,
// but Tomb Raider later moved it to cdcObjects/ which makes more sense IMO.

struct UIActions;
class Instance;
class UberObjectSection;

Instance *UBEROBJECT_BirthSectionInstance(Instance *parent, uint32_t modelIndex, uint32_t id);

class UberObjectComposite : public ObjState {
public:
	uint32_t magic = 0xFEA51B1E; // 50
	Instance **sectionInstances = nullptr; // HACK
private:
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

	void notifyEntry(UberObjectSection*, uint32_t stateIndex);
	void notifyExit(UberObjectSection*, uint32_t stateIndex);
	void notifyTransition(UberObjectSection*, uint32_t transitionIndex);
	void notifyInterruption(UberObjectSection*, uint32_t transitionIndex);

	static UberObjectComposite *GetComposite(Instance*);

	virtual ~UberObjectComposite() = default;
	virtual void commandByName(uint32_t); // 4
	virtual void commandByIndex(int); // 8
	virtual bool methodC(dtp::UberObjectProp::Unknown& entry, int, void*);
	virtual UberObjectSection *createSection(Instance *instance, dtp::UberObjectProp::SectionProp *info, uint32_t index); // 10
	virtual void Update();
	// virtual void write(BinaryWriter&); // 18
	// virtual void read(BinaryReader&); // 1C
};

class UberObjectSection : public ObjState {
public:
	uint32_t magic = 0xF0012345; // 50
	Instance *instance; // 58
	dtp::UberObjectProp::SectionProp *sectionProp; // 5C
	int32_t currentState = -1; // 6C
	float timeInState = 0.0f; // 7C
	uint32_t stateFlags = 0; // 88

public:
	UberObjectSection(Instance *instance, UberObjectComposite *composite, dtp::UberObjectProp::SectionProp *info, uint32_t index);

	void entryActions(bool);
	void exitActions(bool);
	void runActionsLists(
		dtp::UberObjectProp::Action *actions, uint32_t numActions,
		dtp::UberObjectProp::CondAction *condActions, uint32_t numCondActions,
		bool);
	void process();
	void nonVirtualUpdate();
	void takeAutomaticTransitions();
	void takeTransition15(uint32_t);
	void takeTransition18(uint32_t);
	void takeTransitionIfPresent(uint32_t);
	void takeTransition(dtp::UberObjectProp::Transition&);

	dtp::UberObjectProp::Transition *GetUseTransition();
	bool IsUsable();
	void Use();

	static UberObjectSection *GetSection(Instance*);

	virtual ~UberObjectSection() = default;
	virtual void method4(dtp::UberObjectProp::Consequence& conseq);
	// virtual void method8();
	// virtual void methodC();
	virtual void setState(uint32_t, bool); // 10
	virtual bool shouldTakeTransition(dtp::UberObjectProp::Transition& transition);
	virtual void doAction(dtp::UberObjectProp::Action& action);
	// virtual void write(BinaryWriter&); // 1C
	// virtual void read(BinaryReader&); // 20
	// virtual void method24();
	// virtual void method28();
	virtual void Update();
};

#if ENABLE_IMGUI
void buildUI(UIActions&, dtp::UberObjectProp*, Instance *instance=nullptr);
#endif
