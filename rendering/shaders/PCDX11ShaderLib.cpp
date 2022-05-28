#include "PCDX11ShaderLib.h"
#include <cstring>

namespace cdc {

PCDX11ShaderLib::PCDX11ShaderLib(
	char *blob,
	PCDX11ShaderLib::Type type,
	PCDX11RenderDevice *renderDevice,
	bool wineWorkaround)
:
	PCDX11RenderExternalResource(renderDevice)
{
	buffer = nullptr;
	switch (type) {
		case kVertex: // 1
			table = new PCDX11VertexShaderTable(blob, /*takeCopy=*/ false, wineWorkaround); break;
		case kPixel: // 2
			table = new PCDX11PixelShaderTable(blob, /*takeCopy=*/ false); break;
		/*
		case 2:
			table = new PCDX11HullShaderTable(); break;
		case 3:
			table = new PCDX11DomainShaderTable(); break;
		*/
		default: break;
	}
}

void PCDX11ShaderLib::fill(char *data, uint32_t offset, uint32_t size, bool done) {
	memcpy(buffer + offset, data, size);
	if (done) {
		uint32_t libType = *(uint32_t*)buffer;
		switch (libType) {
			case 0:
				table = new PCDX11VertexShaderTable(buffer + 4, /*takeCopy=*/ true); break;
			case 1:
				table = new PCDX11PixelShaderTable(buffer + 4, /*takeCopy=*/ true); break;
			/*
			case 2:
				table = new PCDX11HullShaderTable(); break;
			case 3:
				table = new PCDX11DomainShaderTable(); break;
			*/
			default: break;
		}
		delete buffer;
		buffer = nullptr;
	}
}

}
