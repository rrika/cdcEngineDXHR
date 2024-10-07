#include "VertexDeclaration.h"
#include <cstdlib>
#include <d3d11.h>

namespace cdc {

uint32_t VertexAttributeA::GetSize() {
	switch (format) {
		case 0:
		case 4:
		case 5:
		case 6:
		case 7:
		case 8:
		case 9:
		case 0xB:
		case 0xC:
		case 0xE:
		case 0x10:
		case 0x11:
		case 0x12:
		case 0x13:
			return 4;
		case 1:
		case 0xA:
		case 0xD:
		case 0xF:
		case 0x14:
			return 8;
		case 2:
			return 12;
		case 3:
			return 16;
		default:
			return 0;
	}
}

uint64_t s_byteCRC64[256] = {
	0, 0x42F0E1EBA9EA3693, 0x85E1C3D753D46D26, 0x0C711223CFA3E5BB5,
	0x493366450E42ECDF, 0x0BC387AEA7A8DA4C, 0x0CCD2A5925D9681F9,
	0x8E224479F47CB76A, 0x9266CC8A1C85D9BE, 0x0D0962D61B56FEF2D,
	0x17870F5D4F51B498, 0x5577EEB6E6BB820B, 0x0DB55AACF12C73561,
	0x99A54B24BB2D03F2, 0x5EB4691841135847, 0x1C4488F3E8F96ED4,
	0x663D78FF90E185EF, 0x24CD9914390BB37C, 0x0E3DCBB28C335E8C9,
	0x0A12C5AC36ADFDE5A, 0x2F0E1EBA9EA36930, 0x6DFEFF5137495FA3,
	0x0AAEFDD6DCD770416, 0x0E81F3C86649D3285, 0x0F45BB4758C645C51,
	0x0B6AB559E258E6AC2, 0x71BA77A2DFB03177, 0x334A9649765A07E4,
	0x0BD68D2308226B08E, 0x0FF9833DB2BCC861D, 0x388911E7D1F2DDA8,
	0x7A79F00C7818EB3B, 0x0CC7AF1FF21C30BDE, 0x8E8A101488293D4D,
	0x499B3228721766F8, 0x0B6BD3C3DBFD506B, 0x854997BA2F81E701,
	0x0C7B97651866BD192, 0x0A8546D7C558A27, 0x4258B586D5BFBCB4,
	0x5E1C3D753D46D260, 0x1CECDC9E94ACE4F3, 0x0DBFDFEA26E92BF46,
	0x990D1F49C77889D5, 0x172F5B3033043EBF, 0x55DFBADB9AEE082C,
	0x92CE98E760D05399, 0x0D03E790CC93A650A, 0x0AA478900B1228E31,
	0x0E8B768EB18C8B8A2, 0x2FA64AD7E2F6E317, 0x6D56AB3C4B1CD584,
	0x0E374EF45BF6062EE, 0x0A1840EAE168A547D, 0x66952C92ECB40FC8,
	0x2465CD79455E395B, 0x3821458AADA7578F, 0x7AD1A461044D611C,
	0x0BDC0865DFE733AA9, 0x0FF3067B657990C3A, 0x711223CFA3E5BB50,
	0x33E2C2240A0F8DC3, 0x0F4F3E018F031D676, 0x0B60301F359DBE0E5,
	0x0DA050215EA6C212F, 0x98F5E3FE438617BC, 0x5FE4C1C2B9B84C09,
	0x1D14202910527A9A, 0x93366450E42ECDF0, 0x0D1C685BB4DC4FB63,
	0x16D7A787B7FAA0D6, 0x5427466C1E109645, 0x4863CE9FF6E9F891,
	0x0A932F745F03CE02, 0x0CD820D48A53D95B7, 0x8F72ECA30CD7A324,
	0x150A8DAF8AB144E, 0x43A04931514122DD, 0x84B16B0DAB7F7968,
	0x0C6418AE602954FFB, 0x0BC387AEA7A8DA4C0, 0x0FEC89B01D3679253,
	0x39D9B93D2959C9E6, 0x7B2958D680B3FF75, 0x0F50B1CAF74CF481F,
	0x0B7FBFD44DD257E8C, 0x70EADF78271B2539, 0x321A3E938EF113AA,
	0x2E5EB66066087D7E, 0x6CAE578BCFE24BED, 0x0ABBF75B735DC1058,
	0x0E94F945C9C3626CB, 0x676DD025684A91A1, 0x259D31CEC1A0A732,
	0x0E28C13F23B9EFC87, 0x0A07CF2199274CA14, 0x167FF3EACBAF2AF1,
	0x548F120162451C62, 0x939E303D987B47D7, 0x0D16ED1D631917144,
	0x5F4C95AFC5EDC62E, 0x1DBC74446C07F0BD, 0x0DAAD56789639AB08,
	0x985DB7933FD39D9B, 0x84193F60D72AF34F, 0x0C6E9DE8B7EC0C5DC,
	0x1F8FCB784FE9E69, 0x43081D5C2D14A8FA, 0x0CD2A5925D9681F90,
	0x8FDAB8CE70822903, 0x48CB9AF28ABC72B6, 0x0A3B7B1923564425,
	0x70428B155B4EAF1E, 0x32B26AFEF2A4998D, 0x0F5A348C2089AC238,
	0x0B753A929A170F4AB, 0x3971ED50550C43C1, 0x7B810CBBFCE67552,
	0x0BC902E8706D82EE7, 0x0FE60CF6CAF321874, 0x0E224479F47CB76A0,
	0x0A0D4A674EE214033, 0x67C58448141F1B86, 0x253565A3BDF52D15,
	0x0AB1721DA49899A7F, 0x0E9E7C031E063ACEC, 0x2EF6E20D1A5DF759,
	0x6C0603E6B3B7C1CA, 0x0F6FAE5C07D3274CD, 0x0B40A042BD4D8425E,
	0x731B26172EE619EB, 0x31EBC7FC870C2F78, 0x0BFC9838573709812,
	0x0FD39626EDA9AAE81, 0x3A28405220A4F534, 0x78D8A1B9894EC3A7,
	0x649C294A61B7AD73, 0x266CC8A1C85D9BE0, 0x0E17DEA9D3263C055,
	0x0A38D0B769B89F6C6, 0x2DAF4F0F6FF541AC, 0x6F5FAEE4C61F773F,
	0x0A84E8CD83C212C8A, 0x0EABE6D3395CB1A19, 0x90C79D3FEDD3F122,
	0x0D2377CD44439C7B1, 0x15265EE8BE079C04, 0x57D6BF0317EDAA97,
	0x0D9F4FB7AE3911DFD, 0x9B041A914A7B2B6E, 0x5C1538ADB04570DB,
	0x1EE5D94619AF4648, 0x2A151B5F156289C, 0x4051B05E58BC1E0F,
	0x87409262A28245BA, 0x0C5B073890B687329, 0x4B9237F0FF14C443,
	0x962D61B56FEF2D0, 0x0CE73F427ACC0A965, 0x8C8315CC052A9FF6,
	0x3A80143F5CF17F13, 0x7870F5D4F51B4980, 0x0BF61D7E80F251235,
	0x0FD913603A6CF24A6, 0x73B3727A52B393CC, 0x31439391FB59A55F,
	0x0F652B1AD0167FEEA, 0x0B4A25046A88DC879, 0x0A8E6D8B54074A6AD,
	0x0EA16395EE99E903E, 0x2D071B6213A0CB8B, 0x6FF7FA89BA4AFD18,
	0x0E1D5BEF04E364A72, 0x0A3255F1BE7DC7CE1, 0x64347D271DE22754,
	0x26C49CCCB40811C7, 0x5CBD6CC0CC10FAFC, 0x1E4D8D2B65FACC6F,
	0x0D95CAF179FC497DA, 0x9BAC4EFC362EA149, 0x158E0A85C2521623,
	0x577EEB6E6BB820B0, 0x906FC95291867B05, 0x0D29F28B9386C4D96,
	0x0CEDBA04AD0952342, 0x8C2B41A1797F15D1, 0x4B3A639D83414E64,
	0x9CA82762AAB78F7, 0x87E8C60FDED7CF9D, 0x0C51827E4773DF90E,
	0x20905D88D03A2BB, 0x40F9E43324E99428, 0x2CFFE7D5975E55E2,
	0x6E0F063E3EB46371, 0x0A91E2402C48A38C4, 0x0EBEEC5E96D600E57,
	0x65CC8190991CB93D, 0x273C607B30F68FAE, 0x0E02D4247CAC8D41B,
	0x0A2DDA3AC6322E288, 0x0BE992B5F8BDB8C5C, 0x0FC69CAB42231BACF,
	0x3B78E888D80FE17A, 0x7988096371E5D7E9, 0x0F7AA4D1A85996083,
	0x0B55AACF12C735610, 0x724B8ECDD64D0DA5, 0x30BB6F267FA73B36,
	0x4AC29F2A07BFD00D, 0x8327EC1AE55E69E, 0x0CF235CFD546BBD2B,
	0x8DD3BD16FD818BB8, 0x3F1F96F09FD3CD2, 0x41011884A0170A41,
	0x86103AB85A2951F4, 0x0C4E0DB53F3C36767, 0x0D8A453A01B3A09B3,
	0x9A54B24BB2D03F20, 0x5D45907748EE6495, 0x1FB5719CE1045206,
	0x919735E51578E56C, 0x0D367D40EBC92D3FF, 0x1476F63246AC884A,
	0x568617D9EF46BED9, 0x0E085162AB69D5E3C, 0x0A275F7C11F7768AF,
	0x6564D5FDE549331A, 0x279434164CA30589, 0x0A9B6706FB8DFB2E3,
	0x0EB46918411358470, 0x2C57B3B8EB0BDFC5, 0x6EA7525342E1E956,
	0x72E3DAA0AA188782, 0x30133B4B03F2B111, 0x0F7021977F9CCEAA4,
	0x0B5F2F89C5026DC37, 0x3BD0BCE5A45A6B5D, 0x79205D0E0DB05DCE,
	0x0BE317F32F78E067B, 0x0FCC19ED95E6430E8, 0x86B86ED5267CDBD3,
	0x0C4488F3E8F96ED40, 0x359AD0275A8B6F5, 0x41A94CE9DC428066,
	0x0CF8B0890283E370C, 0x8D7BE97B81D4019F, 0x4A6ACB477BEA5A2A,
	0x89A2AACD2006CB9, 0x14DEA25F3AF9026D, 0x562E43B4931334FE,
	0x913F6188692D6F4B, 0x0D3CF8063C0C759D8, 0x5DEDC41A34BBEEB2,
	0x1F1D25F19D51D821, 0x0D80C07CD676F8394, 0x9AFCE626CE85B507
};

static uint64_t RunCRC64(const char *pData, size_t numBytes) { // line 153
	uint64_t h = 0ull;
	for (size_t i=0; i<numBytes; i++) {
		uint8_t b = pData[i];
		uint8_t c = (b ^ h);
		h = s_byteCRC64[c] ^ (h >> 8);
	}
	return ~h;
}

static int CompareVertexElem(VertexAttributeB const *a, VertexAttributeB const *b) {
	if (a->attribKindA < b->attribKindA)
		return -1;
	if (a->attribKindA > b->attribKindA)
		return 1;
	return 0;
}

void VertexDecl::Finalize() {
	if (hash0 || hash4)
		return;

	uint32_t strides[2] = {0, 0};
	for (uint32_t i=0; i<numAttr; i++) {
		auto *attr = attrib+i;
		uint32_t& currentEnd = strides[attr->bufferIndex];

		if (attr->offset == 0xffff) {
			attr->offset = currentEnd;
			currentEnd += attr->GetSize();

		} else {
			uint32_t beyond = attr->offset + attr->GetSize();
			if (beyond > currentEnd)
				currentEnd = beyond;
		}
	}

	if (vertStrideA == 0) {
		vertStrideA = strides[0];
		vertStrideB = strides[1];
	}

	qsort(attrib, numAttr, sizeof(VertexAttributeA), (int(*)(const void*, const void*))CompareVertexElem);

	uint64_t h = RunCRC64((const char*)&numAttr, 8 + 8*numAttr);

	hash0 = h;
	hash4 = h >> 32;
}

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
