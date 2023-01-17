#include <cstring>
#include "PCShaderLib.h"

namespace cdc {

PCShaderLib::PCShaderLib(
	char *blob,
	bool isPixelShader,
	PCRenderDevice *renderDevice)
:
	PCRenderExternalResource(renderDevice)
{
	buffer = nullptr;
	if (isPixelShader) {
		table = new PCPixelShaderTable(blob, /*takeCopy=*/ false);
		// TODO
	} else {
		table = new PCVertexShaderTable(blob, /*takeCopy=*/ false);
		// TODO
	}
}

void PCShaderLib::fill(char *blob, uint32_t offset, uint32_t size, bool done) {
	memcpy(buffer + offset, blob, size);
	if (done) {
		bool isPixelShader = bool(*(uint32_t*)buffer);
		if (isPixelShader) {
			table = new PCPixelShaderTable(blob, /*takeCopy=*/ true);
			// TODO
		} else {
			table = new PCVertexShaderTable(blob, /*takeCopy=*/ true);
		}
		delete[] buffer;
		buffer = nullptr;
	}
}

}
