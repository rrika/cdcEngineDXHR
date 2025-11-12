#include "PCDX11Shared.h"

namespace cdc {

uint32_t ConvertFormatD3D9ToDXGI(uint32_t d3dformat) {
	switch (d3dformat) {
		case '1TXD': // 0x31545844
			return 71; // DXGI_FORMAT_BC1_UNORM
		case '2TXD':
			return 71; // DXGI_FORMAT_BC1_UNORM
		case '3TXD':
			return 74; // DXGI_FORMAT_BC2_UNORM
		case '4TXD':
			return 74; // DXGI_FORMAT_BC2_UNORM
		case '5TXD':
			return 77; // DXGI_FORMAT_BC3_UNORM
		case 20:
		case 21:
		case 22:
		case 26:
		case 27:
		case 29:
		case 30:
			return 28; // DXGI_FORMAT_R8G8B8A8_UNORM
		case 23:
			return 85; // DXGI_FORMAT_B5G6R5_UNORM
		case 24:
		case 25:
			return 86; // DXGI_FORMAT_B5G5R5A1_UNORM
		case 28:
			return 65; // DXGI_FORMAT_A8_UNORM
		case 31:
		case 35:
			return 24; // DXGI_FORMAT_R10G10B10A2_UNORM
		case 32:
		case 33:
			return 87; // DXGI_FORMAT_B8G8R8A8_UNORM
		case 34:
			return 35; // DXGI_FORMAT_R16G16_UNORM
		case 36:
			return 11; // DXGI_FORMAT_R16G16B16A16_UNORM
		case 111:
			return 54; // DXGI_FORMAT_R16_FLOAT
		case 112:
			return 34; // DXGI_FORMAT_R16G16_FLOAT
		case 113:
			return 10; // DXGI_FORMAT_R16G16B16A16_FLOAT
		case 114:
			return 41; // DXGI_FORMAT_R32_FLOAT
		case 115:
			return 16; // DXGI_FORMAT_R32G32_FLOAT
		case 116:
			return 2; // DXGI_FORMAT_R32G32B32A32_FLOAT
		default:
			return 0; // DXGI_FORMAT_UNKNOWN
	}
}

}
