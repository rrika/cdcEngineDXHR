#include "VertexAttribute.h"
#include <d3d11.h>

namespace cdc {

static DXGI_FORMAT decodeFormat(uint16_t format) {
	switch (format) {
		case 0: return DXGI_FORMAT_R32_FLOAT; // 41
		case 1: return DXGI_FORMAT_R32G32_FLOAT; // 16
		case 2: return DXGI_FORMAT_R32G32B32_FLOAT; // 6
		case 3: return DXGI_FORMAT_R32G32B32A32_FLOAT; // 2
		case 4:
		case 5:
		case 6:
		case 7:
		case 11:
			return DXGI_FORMAT_R8G8B8A8_UNORM; // 28
		case 8: return DXGI_FORMAT_R8G8B8A8_UINT; // 30
		case 12:
		case 19:
			return DXGI_FORMAT_R16G16_SNORM; // 37
		case 9: return DXGI_FORMAT_R16G16B16A16_TYPELESS; // 38
		case 13:
		case 20:
			return DXGI_FORMAT_R16G16B16A16_SNORM; // 13
		case 10:
			return DXGI_FORMAT_R16G16B16A16_SINT; // 14
		case 14:
			return DXGI_FORMAT_R16G16_UNORM; // 35
		case 15:
			return DXGI_FORMAT_R16G16B16A16_UNORM; // 11
		case 16:
			return DXGI_FORMAT_R10G10B10A2_UINT; // 25
		case 17:
			return DXGI_FORMAT_R10G10B10A2_UNORM; // 24
		default:
			return DXGI_FORMAT_UNKNOWN;
	}
}

static const uint32_t Position_crc32 = 0xD2F7D823;
static const uint32_t Normal_crc32 = 0x36F5E414;
static const uint32_t Texcoord1_crc32 = 0x8317902A;
static const uint32_t Texcoord2_crc32 = 0x8E54B6F3;
static const uint32_t Binormal_crc32 = 0x64A86F01;
static const uint32_t Tangent_crc32 = 0XF1ED11C3;
static const uint32_t Color_crc32 = 0XFFFFFFFF; // hack

void decodeVertexAttribA(D3D11_INPUT_ELEMENT_DESC *dst, VertexAttributeA *src, uint32_t count, bool wineWorkaround) {
	uint32_t customSlots = 0;
	for (uint32_t i=0; i<count; i++) {
		dst[i].SemanticName = "";
		dst[i].InputSlot = 0;
		dst[i].AlignedByteOffset = src[i].offset;
		dst[i].Format = decodeFormat(src[i].format);
		auto& elem = dst[i];
		auto kind = src[i].attribKind;
		if (kind == Position_crc32) {
			elem.SemanticName = wineWorkaround ? "SV_POSITION" : "POSITION";
			elem.SemanticIndex = 0;
		} else if (kind == Normal_crc32) {
			elem.SemanticName = "NORMAL";
			elem.SemanticIndex = 0;
		} else if (kind == Tangent_crc32) {
			elem.SemanticName = "TANGENT";
			elem.SemanticIndex = 0;
		} else if (kind == Binormal_crc32) {
			elem.SemanticName = "BINORMAL";
			elem.SemanticIndex = 0;
		} else if (kind == Texcoord1_crc32) {
			elem.SemanticName = "TEXCOORD";
			elem.SemanticIndex = 1;
		} else if (kind == Texcoord2_crc32) {
			elem.SemanticName = "TEXCOORD";
			elem.SemanticIndex = 2;
		} else if (kind == Color_crc32) {
			elem.SemanticName = "COLOR";
			elem.SemanticIndex = 0;
		} else {
			elem.SemanticName = "";
			elem.SemanticIndex = customSlots++;
		}
		// printf("%s[%d] at offset %d with format %d (%d)\n", elem.SemanticName, elem.SemanticIndex, elem.AlignedByteOffset, elem.Format, src[i].format);
	}
}

}
