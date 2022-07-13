#include "buffers/PCDX11UberConstantBuffer.h"
#include "PCDX11DeviceManager.h"
#include "PCDX11StreamDecl.h"
#include "PCDX11StreamDeclCache.h"
#include "shaders/PCDX11Shader.h"
#include "VertexAttribute.h"

namespace cdc {

DXGI_FORMAT decodeFormat(uint16_t format);

PCDX11StreamDecl *PCDX11StreamDeclCache::buildStreamDecl(
	VertexAttributeLayoutA *layout,
	PCDX11ShaderSub *shaderSub)
{
	StreamDeclCacheKey key {0, layout->hash0};
	auto it = cache.insert({key, nullptr});
	PCDX11StreamDecl *&streamDecl = it.first->second;

	if (it.second) {
		D3D11_INPUT_ELEMENT_DESC *inputElementDesc = new D3D11_INPUT_ELEMENT_DESC[layout->numAttr];
		memset(inputElementDesc, 0, sizeof(D3D11_INPUT_ELEMENT_DESC[layout->numAttr]));
		decodeVertexAttribA(inputElementDesc, layout->attrib, layout->numAttr, shaderSub->wineWorkaround);
		streamDecl = new PCDX11StreamDecl(renderDevice, inputElementDesc, layout->numAttr, shaderSub);

		// HACK
		auto buffer = new PCDX11UberConstantBuffer(2);
		float rows[] = {
			2.0f, -1.0f, 0.0f, 0.0f, // NormalScaleOffset
			1.0f,  0.0f, 0.0f, 0.0f  // TexcoordScales
		};
		buffer->assignRow(0, rows, 2);
		buffer->syncBuffer(deviceManager->getD3DDeviceContext());
		streamDecl->streamDeclBuffer = buffer;
	}

	return streamDecl;
}

PCDX11StreamDecl *PCDX11StreamDeclCache::buildStreamDecl(
	VertexAttributeLayoutA *layoutA,
	VertexAttributeLayoutB *layoutB,
	bool a4,
	PCDX11ShaderSub *shaderSub)
{
	// a4 = include normal, binormal, tangent, etc.

	if (!layoutB)
		return buildStreamDecl(layoutA, shaderSub); // HACK

	StreamDeclCacheKey key {layoutB->hash0, layoutA->hash0};
	if (a4) key.hashB = ~key.hashB;
	auto it = cache.insert({key, nullptr});
	PCDX11StreamDecl *&streamDecl = it.first->second;

	if (it.second) {
		// allocate more than necessary, not all slots will be used
		D3D11_INPUT_ELEMENT_DESC *inputElementDesc = new D3D11_INPUT_ELEMENT_DESC[layoutB->numAttribs];
		memset(inputElementDesc, 0, sizeof(D3D11_INPUT_ELEMENT_DESC[layoutB->numAttribs]));

		uint32_t forbiddenBit = a4 ? 4 : 2;
		uint32_t indexB;
		uint32_t numElements = 0;
		// uint32_t secondaryBufferByteOffset = 0;
		VertexAttributeB *attribB = &layoutB->attr[0];
		for (uint32_t i=0; i<layoutB->numAttribs; i++) {
			indexB = i;
			if ((attribB->field_A & forbiddenBit) == 0) {

				printf("matching layoutB[%d] (next=%d) with\n", i, attribB->nextAttribIndex);

				// find match in layoutA, including fallbacks
				uint16_t indexA = 0xffff;
				while (indexB != -1) {
					uint32_t hash = layoutB->attr[indexB].attribKindA;
					indexA = getLayoutAIndexFromHash(layoutA, hash);

					printf("  layoutB[%d] %08x -> layoutA[%d]\n", indexB, hash, indexA);

					if (indexA != 0xffff)
						break;

					// next attempt
					indexB = layoutB->attr[indexB].nextAttribIndex;
				}

				if (indexB != -1) {
					// input from vertex buffer

					inputElementDesc[numElements].InputSlot = 0;
					inputElementDesc[numElements].AlignedByteOffset = layoutA->attrib[indexA].offset;
					inputElementDesc[numElements].Format = decodeFormat(layoutA->attrib[indexA].format);

					semanticFromEnum(inputElementDesc+numElements, attribB->attribKindB);
					printf("    %s %d offset=%d\n",
						inputElementDesc[numElements].SemanticName,
						inputElementDesc[numElements].SemanticIndex);
					numElements++;
				} else {
					// input from secondary buffer

					// TODO
					// inputElementDesc[numElements].AlignedByteOffset = secondaryBufferByteOffset;
					// secondaryBufferByteOffset += 16;
					// elem++;
					// numElements++;
					// semanticFromEnum(elem, attribB->attribKindB);
				}
			} else {
				printf("skipping layoutB[%d] (next=%d)\n", i, attribB->nextAttribIndex);
			}
			attribB++;
		}

		for (uint32_t i=0; i<numElements; i++) {
			D3D11_INPUT_ELEMENT_DESC& elem = inputElementDesc[i];
			printf("%s[%d] at offset %d with format %d\n", elem.SemanticName, elem.SemanticIndex, elem.AlignedByteOffset, elem.Format);
		}


		streamDecl = new PCDX11StreamDecl(renderDevice, inputElementDesc, numElements, shaderSub);

		// HACK
		auto buffer = new PCDX11UberConstantBuffer(2);
		float rows[] = {
			2.0f, -1.0f, 0.0f, 0.0f, // NormalScaleOffset
			16.0f, 16.0f, 0.0f, 0.0f  // TexcoordScales
		};
		buffer->assignRow(0, rows, 2);
		buffer->syncBuffer(deviceManager->getD3DDeviceContext());
		streamDecl->streamDeclBuffer = buffer;
	}

	return streamDecl;
}

}
