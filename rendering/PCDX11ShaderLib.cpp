#include "PCDX11ShaderLib.h"
#include <cstring>

namespace cdc {

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
