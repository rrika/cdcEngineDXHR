#include "ResolveSection.h"

#include "AnimationSection.h"
#include "DTPDataSection.h"
#include "GenericSection.h"
#include "MaterialSection.h"
#include "ObjectSection.h"
#include "RenderResourceSection.h"
#include "ScriptSection.h"
#include "ShaderLibSection.h"
#include "WaveSection.h"

namespace cdc {

cdc::ResolveSection *g_resolveSections[16] = {nullptr};

void ResolveSection::Create() {
	g_resolveSections[0] = new cdc::GenericSection();
	g_resolveSections[2] = new cdc::AnimationSection();
	g_resolveSections[5] = new cdc::RenderResourceSection();
	g_resolveSections[6] = new cdc::WaveSection();
	g_resolveSections[7] = new DTPDataSection();
	g_resolveSections[8] = new cdc::ScriptSection();
	g_resolveSections[9] = new cdc::ShaderLibSection();
	g_resolveSections[10] = new cdc::MaterialSection();
	g_resolveSections[11] = new cdc::ObjectSection();
	g_resolveSections[12] = new cdc::RenderResourceSection();
	g_resolveSections[13] = new cdc::GenericSection(); // HACK, for collision data
}

void ResolveSection::Destroy() {
	delete g_resolveSections[0];
	delete g_resolveSections[2];
	delete g_resolveSections[5];
	delete g_resolveSections[6];
	delete g_resolveSections[7];
	delete g_resolveSections[8];
	delete g_resolveSections[9];
	delete g_resolveSections[10];
	delete g_resolveSections[11];
	delete g_resolveSections[12];
}

ResolveSection::~ResolveSection() {}

uint32_t ResolveSection::StartResource(
	uint32_t sectionId,
	uint32_t unknown,
	uint32_t size, 
	bool& alreadyLoaded)
{
	return -1;
}

uint32_t ResolveSection::StartResource(
	uint32_t sectionId,
	uint32_t flags,
	uint32_t unknown,
	uint32_t size,
	bool& alreadyLoaded)
{
	return StartResource(sectionId, unknown, size, alreadyLoaded);
}

void *ResolveSection::GetResolveBasePointer(uint32_t sectionId) {
	return GetBasePointer(sectionId);
}

}
