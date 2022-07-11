#include "CommonRenderDevice.h"
#include "surfaces/TextureMap.h"

namespace cdc {

CommonRenderDevice *g_renderDevice = nullptr;

CommonRenderDevice::CommonRenderDevice() :
	linear4(0x20000, false, /*&linearAllocExt,*/ "RenderDevice")
{
	linear30 = new LinearAllocator(0x300000, false, "RenderDevice");
	linear34 = new LinearAllocator(0x300000, false, "RenderDevice");
	// ensure that the draw function 0 in every func set just calls IRenderDrawable::draw
	renderPasses.allocFuncIndex("Default");
}

void CommonRenderDevice::method_14() {
	// TODO
}

void CommonRenderDevice::method_18() {
	// TODO
}

void CommonRenderDevice::method_34() {
	// TODO
}

void CommonRenderDevice::method_38() {
	// TODO
}

void CommonRenderDevice::method_3C() {
	// TODO
}

void CommonRenderDevice::method_40() {
	// TODO
}

void CommonRenderDevice::method_44() {
	// TODO
}

void CommonRenderDevice::createSiblingScene(
	CommonSceneSub10 *sub10,
	CommonRenderTarget *renderTarget,
	CommonDepthBuffer *depthBuffer)
{
	// TODO
	createSubScene(sub10, renderTarget, depthBuffer);
	// TODO
}

void CommonRenderDevice::method_54() {
	// TODO
}

void CommonRenderDevice::method_58() {
	// TODO
}

void CommonRenderDevice::getScene() {
	// TODO
}

void CommonRenderDevice::method_68() {
	// TODO
}

void CommonRenderDevice::addPass(uint32_t arg0, uint32_t order, uint32_t sortMode, uint32_t funcSetIndex) {
	renderPasses.addRenderPass(arg0, order, sortMode, funcSetIndex, /*firstPassId=*/0);
}

void CommonRenderDevice::method_78() {
	// TODO
}

uint32_t CommonRenderDevice::getPassOrder(uint32_t passId) {
	return renderPasses.passes[passId].order;
}

void CommonRenderDevice::method_80() {
	// TODO
}

void CommonRenderDevice::setPassCallback(uint32_t passId, IRenderPassCallback *cb) {
	renderPasses.passes[passId].callbacks = cb;
}

void CommonRenderDevice::method_88() {
	// TODO
}

void CommonRenderDevice::method_8C() {
	// TODO
}

uint8_t CommonRenderDevice::allocFuncIndex(const char *name) {
	return renderPasses.allocFuncIndex(name);
}

void CommonRenderDevice::freeFuncIndex(uint8_t funcSet) {
	// TODO
}

void CommonRenderDevice::registerDrawer(uint32_t funcIndex, uint32_t funcSetIndex, RenderFunc drawer) {
	renderPasses.drawers[funcSetIndex].func[funcIndex] = drawer;
}

RenderFunc CommonRenderDevice::getDrawer(uint32_t funcIndex, uint32_t funcSetIndex) {
	return renderPasses.drawers[funcSetIndex].func[funcIndex];
}

void CommonRenderDevice::registerComparator(uint32_t funcIndex, uint32_t funcSetIndex, RenderFunc comparator) {
	renderPasses.comparators[funcSetIndex].func[funcIndex] = comparator;
}

RenderFunc CommonRenderDevice::getComparator(uint32_t funcIndex, uint32_t funcSetIndex) {
	return renderPasses.comparators[funcSetIndex].func[funcIndex];
}

void CommonRenderDevice::method_AC() {
	// TODO
}

void CommonRenderDevice::method_B0() {
	// TODO
}

void CommonRenderDevice::method_B4() {
	// TODO
}

void CommonRenderDevice::method_E0() {
	// TODO
}

void CommonRenderDevice::method_E4() {
	// TODO
}

CommonSceneSub114 *CommonRenderDevice::getSceneSub114() {
	if (scene7C)
		return &scene7C->sub114;
	else
		return &sceneSub114_8C;
	return nullptr;
}

void CommonRenderDevice::method_F0() {
	// TODO
}

void CommonRenderDevice::method_F4() {
	// TODO
}

void CommonRenderDevice::method_F8() {
	// TODO
}

void CommonRenderDevice::method_FC() {
	// TODO
}

void CommonRenderDevice::method_100() {
	// TODO
}

void CommonRenderDevice::method_104() {
	// TODO
}

void CommonRenderDevice::method_108() {
	// TODO
}

void CommonRenderDevice::method_10C() {
	// TODO
}

void CommonRenderDevice::method_110() {
	// TODO
}

void CommonRenderDevice::method_114() {
	// TODO
}

void CommonRenderDevice::method_118() {
	// TODO
}

void CommonRenderDevice::method_11C() {
	// TODO
}

void CommonRenderDevice::method_120() {
	// TODO
}

void CommonRenderDevice::method_124() {
	// TODO
}

void *CommonRenderDevice::linearAlloc30(uint32_t size, uint32_t requester) {
	return (void*)linear30->alloc(size, requester, /*reportFailure=*/true);
}

RenderResource *CommonRenderDevice::method_140(uint32_t) {
	// TODO
	return nullptr;
}

void CommonRenderDevice::method_184() {
	// TODO
}

bool CommonRenderDevice::isInStereoMode() {
	// TODO
	return false;
}

void CommonRenderDevice::method_18C() {
	// TODO
}

void CommonRenderDevice::method_190() {
	// TODO
}

bool CommonRenderDevice::useAlternateLinearAlloc() {
	// TODO
	return false;
}

void CommonRenderDevice::method_198() {
	// TODO
}

void CommonRenderDevice::method_19C() {
	// TODO
}

void CommonRenderDevice::method_1A0() {
	// TODO
}

RenderResource *CommonRenderDevice::createResource(uint32_t type, uint32_t arg) {
	switch (type) {
		case 5: return g_renderDevice->createTexture(arg);
		case 24: return g_renderDevice->createRenderTerrain(arg);
		case 26: return g_renderDevice->createRenderModel(arg);
		case 27: return g_renderDevice->method_140(arg);
		default: return nullptr;
	}
}

}
