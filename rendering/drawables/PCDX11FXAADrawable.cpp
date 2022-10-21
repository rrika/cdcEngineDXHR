#include "PCDX11FXAADrawable.h"
#include "../PCDX11RenderDevice.h"

namespace cdc {

void PCDX11FXAADrawable::draw(uint32_t funcSetIndex, IRenderDrawable *other) {
	(void)funcSetIndex;
	(void)other;
	renderDevice->doFXAA(quality, texture, renderTarget);
}

uint32_t PCDX11FXAADrawable::compare(uint32_t funcSetIndex, IRenderDrawable *other) {
	// TODO
	return 1;
}

}
