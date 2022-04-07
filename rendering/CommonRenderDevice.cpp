#include "CommonRenderDevice.h"
#include "TextureMap.h"

namespace cdc {

CommonRenderDevice *gRenderDevice = nullptr;

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

void CommonRenderDevice::method_4C() {
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

void CommonRenderDevice::method_74() {
	// TODO
}

void CommonRenderDevice::method_78() {
	// TODO
}

void CommonRenderDevice::method_7C() {
	// TODO
}

void CommonRenderDevice::method_80() {
	// TODO
}

void CommonRenderDevice::registerPassCallback() {
	// TODO
}

void CommonRenderDevice::method_88() {
	// TODO
}

void CommonRenderDevice::method_8C() {
	// TODO
}

void CommonRenderDevice::method_90() {
	// TODO
}

void CommonRenderDevice::method_94() {
	// TODO
}

void CommonRenderDevice::registerDrawer() {
	// TODO
}

void CommonRenderDevice::getDrawer() {
	// TODO
}

void CommonRenderDevice::registerComparator() {
	// TODO
}

void CommonRenderDevice::getComparator() {
	// TODO
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

void CommonRenderDevice::method_E8() {
	// TODO
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

void CommonRenderDevice::method_128() {
	// TODO
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

void CommonRenderDevice::method_194() {
	// TODO
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
		case 5: return gRenderDevice->createTexture(arg);
		case 24: return gRenderDevice->createRenderTerrain(arg);
		case 26: return gRenderDevice->createRenderModel(arg);
		case 27: return gRenderDevice->method_140(arg);
		default: return nullptr;
	}
}

}
