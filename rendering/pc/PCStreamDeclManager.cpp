#include <d3d9.h>
#include "rendering/pc/PCStreamDecl.h"
#include "rendering/pc/PCStreamDeclManager.h"
#include "rendering/pc/buffers/PCStaticVertexBuffer.h"
#include "rendering/VertexDeclaration.h"

namespace cdc {

PCStreamDecl *PCStreamDeclManager::FindOrCreate(VertexDecl *layout)
{
	StreamDeclCacheKey9 key {0, layout->hash0};
	auto it = cache.insert({key, nullptr});
	PCStreamDecl *&streamDecl = it.first->second;

	if (it.second) {
		D3DVERTEXELEMENT9 *elements = new D3DVERTEXELEMENT9[layout->numAttr + 1];
		memset(elements, 0, sizeof(D3DVERTEXELEMENT9) * layout->numAttr);
		MakeD3DVertexElements(elements, layout->attrib, layout->numAttr);
		streamDecl = new PCStreamDecl(elements, layout->vertStrideA);
	}

	return streamDecl;
}

D3DDECLTYPE MakeD3DDeclType(uint16_t format);

PCStreamDecl *PCStreamDeclManager::FindOrCreate(
	VertexDecl *vertexDecl,
	ShaderInputSpec *inputSpec,
	bool a4)
{
	// TODO: this whole function is a slightly modified version of the DX11 equivalent
	//       need to take a proper look at it some time

	// a4 = include normal, binormal, tangent, etc.
	Vector4 normalScaleOffset{2.0f, -1.0f, 0.0f, 0.0f};
	Vector4 texcoordScales{0.0f, 0.0f, 0.0f, 0.0f};
	Vector4 secondaryData[512 / 16];

	if (!inputSpec)
		return FindOrCreate(vertexDecl); // HACK

	StreamDeclCacheKey9 key {inputSpec->hash0, vertexDecl->hash0};
	if (a4) key.hashB = ~key.hashB;
	auto it = cache.insert({key, nullptr});
	PCStreamDecl *&streamDecl = it.first->second;

	if (it.second) {
		// allocate more than necessary, not all slots will be used
		D3DVERTEXELEMENT9 *vertexElem = new D3DVERTEXELEMENT9[inputSpec->numAttribs + 1];
		memset(vertexElem, 0, sizeof(D3DVERTEXELEMENT9) * inputSpec->numAttribs);

		uint32_t forbiddenBit = a4 ? 2 : 4;
		uint32_t indexB;
		uint32_t numElements = 0;
		uint32_t secondaryBufferByteOffset = 0;
		uint32_t secondaryBufferCount = 0;
		VertexAttributeB *attribB = &inputSpec->attr[0];
		for (uint32_t i=0; i<inputSpec->numAttribs; i++) {
			indexB = i;
			if ((attribB->field_A & forbiddenBit) == 0) {

				// printf("matching inputSpec[%d] (next=%d) with\n", i, attribB->nextAttribIndex);

				// find match in vertexDecl, including fallbacks
				uint16_t indexA = 0xffff;
				while (indexB != -1) {
					uint32_t hash = inputSpec->attr[indexB].attribKindA;
					indexA = getLayoutAIndexFromHash(vertexDecl, hash);

					// printf("  inputSpec[%d] %08x -> vertexDecl[%d]\n", indexB, hash, indexA);

					if (indexA != 0xffff)
						break;

					// next attempt
					indexB = inputSpec->attr[indexB].nextAttribIndex;
				}

				if (indexB != -1) {
					// input from vertex buffer

					vertexElem[numElements].Stream = 0;
					vertexElem[numElements].Offset = vertexDecl->attrib[indexA].offset;
					vertexElem[numElements].Type = MakeD3DDeclType(vertexDecl->attrib[indexA].format);

					if (attribB->attribKindB >= 10 && attribB->attribKindB < 14) {
						if (vertexDecl->attrib[indexA].format < 19 ||
							vertexDecl->attrib[indexA].format > 20
						)
							texcoordScales.vec128[attribB->attribKindB-10] = 1.0f;
						else
							texcoordScales.vec128[attribB->attribKindB-10] = 0.00048828125f; // contrast with 15.999512f used in DX11
					}
					if (attribB->attribKindB == 4 && vertexDecl->attrib[indexA].format == 2) {
						normalScaleOffset.x = 1.0f;
						normalScaleOffset.y = 0.0f;
						normalScaleOffset.z = 0.0f;
						normalScaleOffset.w = 0.0f;
					}

				} else {
					// input from secondary buffer

					vertexElem[numElements].Offset = secondaryBufferByteOffset;
					vertexElem[numElements].Stream = 1;
					vertexElem[numElements].Type = D3DDECLTYPE_FLOAT4; // 3
					secondaryData[secondaryBufferCount] = Vector4{
						attribB->vx,
						attribB->vy,
						attribB->vz,
						attribB->vw,
					};
					secondaryBufferCount++;
					secondaryBufferByteOffset += 16;

					if (attribB->attribKindB >= 10 && attribB->attribKindB < 14)
						texcoordScales.vec128[attribB->attribKindB-10] = 1.0f;

				}
				vertexElem[numElements].Method = D3DDECLMETHOD_DEFAULT; // 0

				semanticFromEnum(vertexElem+numElements, attribB->attribKindB);
				// printf("    %s %d offset=%d\n",
				//	vertexElem[numElements].SemanticName,
				//	vertexElem[numElements].SemanticIndex);
				numElements++;

			} else {
				// printf("skipping inputSpec[%d] (next=%d)\n", i, attribB->nextAttribIndex);
			}
			attribB++;
		}
		vertexElem[numElements] = { 255, 0, D3DDECLTYPE_UNUSED /*17*/, 0, 0, 0 };

		for (uint32_t i=0; i<numElements; i++) {
			D3DVERTEXELEMENT9& elem = vertexElem[i];
			// printf("%d[%d] at offset %d with type %d\n", elem.Usage, elem.UsageIndex, elem.Offset, elem.Type);
		}


		streamDecl = new PCStreamDecl(vertexElem, vertexDecl->vertStrideA);

		/*
		printf("NormalScaleOffset %f %f %f %f\n",
			normalScaleOffset.x,
			normalScaleOffset.y,
			normalScaleOffset.z,
			normalScaleOffset.w);

		printf("TexcoordScales %f %f %f %f\n",
			texcoordScales.x,
			texcoordScales.y,
			texcoordScales.z,
			texcoordScales.w);
		*/

		streamDecl->normalScaleOffset = normalScaleOffset;
		streamDecl->texcoordScales = texcoordScales;

		if (secondaryBufferCount) {
			auto newData = new Vector4[secondaryBufferCount];
			streamDecl->secondaryData = newData;
			memcpy(newData, secondaryData, secondaryBufferCount * 4);
			auto *buffer = new PCStaticVertexBuffer(nullptr);
			buffer->Create(newData, 1, 16 * secondaryBufferCount);
			streamDecl->secondaryVertexBuffer = buffer;
		}
	}

	return streamDecl;
}

}
