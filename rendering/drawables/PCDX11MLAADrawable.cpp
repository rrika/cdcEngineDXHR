#include "PCDX11MLAADrawable.h"
#include "../PCDX11RenderDevice.h"

namespace cdc {

void PCDX11MLAADrawable::draw(uint32_t funcSetIndex, IRenderDrawable *other) {
	(void)funcSetIndex;
	(void)other;
	renderDevice->ApplyMLAA(texture, renderTarget);
}

uint32_t PCDX11MLAADrawable::compare(uint32_t funcSetIndex, IRenderDrawable *other) {
	// TODO
	return 1;
}

}
