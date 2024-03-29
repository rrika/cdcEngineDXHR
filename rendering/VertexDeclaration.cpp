#include "VertexDeclaration.h"
#include <d3d11.h>

namespace cdc {

uint16_t getLayoutAIndexFromHash(
	VertexDecl *layoutA,
	uint32_t hash)
{
	for (uint32_t i=0; i<layoutA->numAttr; i++) {
		if (layoutA->attrib[i].attribKind == hash)
			return (uint16_t)i;
	}
	return 0xffff;
}

DXGI_FORMAT MakeElementFormat(uint16_t format) {
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

// very confusingly, the mesh will sometimes have two inputs called
// Texcoord1 and Texcoord2, which when decoded using MakeD3DVertexElements
// will turn into the same semantics in d3d.
// But when using semanticFromEnum they will be mapped to Texcoord0 and
// Texcoord1, which is what the pixel shader expects.

void MakeD3DVertexElements(D3D11_INPUT_ELEMENT_DESC *dst, VertexAttributeA *src, uint32_t count, bool wineWorkaround) {
	uint32_t customSlots = 0;
	for (uint32_t i=0; i<count; i++) {
		dst[i].SemanticName = "";
		dst[i].InputSlot = 0;
		dst[i].AlignedByteOffset = (uint32_t)(int32_t)(int16_t)src[i].offset; // 0xffff -> 0xffffffff
		dst[i].Format = MakeElementFormat(src[i].format);
		auto& elem = dst[i];
		auto kind = src[i].attribKind;
		if (kind == VertexAttributeA::kPosition) {
			elem.SemanticName = wineWorkaround ? "SV_POSITION" : "POSITION";
			elem.SemanticIndex = 0;
		} else if (kind == VertexAttributeA::kNormal) {
			elem.SemanticName = "NORMAL";
			elem.SemanticIndex = 0;
		} else if (kind == VertexAttributeA::kTangent) {
			elem.SemanticName = "TANGENT";
			elem.SemanticIndex = 0;
		} else if (kind == VertexAttributeA::kBinormal) {
			elem.SemanticName = "BINORMAL";
			elem.SemanticIndex = 0;
		} else if (kind == VertexAttributeA::kTexcoord0) { // HACK
			elem.SemanticName = "TEXCOORD";
			elem.SemanticIndex = 0;
		} else if (kind == VertexAttributeA::kTexcoord1) {
			elem.SemanticName = "TEXCOORD";
			elem.SemanticIndex = 1;
		} else if (kind == VertexAttributeA::kTexcoord2) {
			elem.SemanticName = "TEXCOORD";
			elem.SemanticIndex = 2;
		} else if (kind == VertexAttributeA::kColor1) {
			elem.SemanticName = "COLOR";
			elem.SemanticIndex = 0;
		} else {
			elem.SemanticName = "";
			elem.SemanticIndex = customSlots++;
		}
	}
}

void semanticFromEnum(D3D11_INPUT_ELEMENT_DESC *elem, int e) {
	switch (e) {
		case 1:
			elem->SemanticName = "POSITION";
			elem->SemanticIndex = 0;
			break;
		case 2:
			elem->SemanticName = "BLENDWEIGHT";
			elem->SemanticIndex = 0;
			break;
		case 3:
			elem->SemanticName = "BLENDINDICES";
			elem->SemanticIndex = 0;
			break;
		case 4:
			elem->SemanticName = "NORMAL";
			elem->SemanticIndex = 0;
			break;
		case 7:
			elem->SemanticName = "NORMAL";
			elem->SemanticIndex = 1;
			break;
		case 5:
			elem->SemanticName = "BINORMAL";
			elem->SemanticIndex = 0;
			break;
		case 6:
			elem->SemanticName = "TANGENT";
			elem->SemanticIndex = 0;
			break;
		case 8:
			elem->SemanticName = "COLOR";
			elem->SemanticIndex = 0;
			break;
		case 9:
			elem->SemanticName = "COLOR";
			elem->SemanticIndex = 1;
			break;
		case 10:
			elem->SemanticIndex = 0;
			elem->SemanticName = "TEXCOORD";
			break;
		case 11:
			elem->SemanticIndex = 1;
			elem->SemanticName = "TEXCOORD";
			break;
		case 12:
			elem->SemanticIndex = 2;
			elem->SemanticName = "TEXCOORD";
			break;
		case 13:
			elem->SemanticIndex = 3;
			elem->SemanticName = "TEXCOORD";
			break;
		case 14:
			elem->SemanticIndex = 4;
			elem->SemanticName = "TEXCOORD";
			break;
		case 15:
			elem->SemanticIndex = 5;
			elem->SemanticName = "TEXCOORD";
			break;
		case 16:
			elem->SemanticIndex = 6;
			elem->SemanticName = "TEXCOORD";
			break;
		case 17:
			elem->SemanticIndex = 7;
			elem->SemanticName = "TEXCOORD";
			break;
		default:
			break;
	}
}

}
