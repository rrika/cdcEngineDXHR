#include "ResolveSection.h"
#include "sections/DTPDataSection.h"
#include "sections/GenericSection.h"
#include "sections/MaterialSection.h"
#include "sections/ObjectSection.h"
#include "sections/RenderResourceSection.h"
#include "sections/ShaderLibSection.h"
#include "sections/WaveSection.h"

namespace cdc {

cdc::ResolveSection *g_resolveSections[16] = {nullptr};

void ResolveSection::Create() {
	g_resolveSections[0] = new cdc::GenericSection();
	g_resolveSections[5] = new cdc::RenderResourceSection();
	g_resolveSections[6] = new cdc::WaveSection();
	g_resolveSections[7] = new cdc::DTPDataSection();
	g_resolveSections[9] = new cdc::ShaderLibSection();
	g_resolveSections[10] = new cdc::MaterialSection();
	g_resolveSections[11] = new cdc::ObjectSection();
	g_resolveSections[12] = new cdc::RenderResourceSection();
}

ResolveSection::~ResolveSection() {}

uint32_t ResolveSection::realize(
	uint32_t sectionId,
	uint32_t unknown,
	uint32_t size, 
	bool& alreadyLoaded)
{
	return -1;
}

uint32_t ResolveSection::allocate(
	uint32_t sectionId,
	uint32_t flags,
	uint32_t unknown,
	uint32_t size,
	bool& alreadyLoaded)
{
	return realize(sectionId, unknown, size, alreadyLoaded);
}

void *ResolveSection::getBlob(uint32_t sectionId) {
	return getWrapped(sectionId);
}

}
