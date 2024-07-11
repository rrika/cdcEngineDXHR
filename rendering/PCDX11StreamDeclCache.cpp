#include "cdcMath/Math.h"
#include "buffers/PCDX11SimpleStaticVertexBuffer.h"
#include "buffers/PCDX11UberConstantBuffer.h"
#include "PCDX11DeviceManager.h"
#include "PCDX11StreamDecl.h"
#include "PCDX11StreamDeclCache.h"
#include "shaders/PCDX11Shader.h"
#include "VertexDeclaration.h"

namespace cdc {

DXGI_FORMAT MakeElementFormat(uint16_t format);

PCDX11StreamDecl *PCDX11StreamDeclCache::buildStreamDecl(
	VertexDecl *layout,
	PCDX11ShaderBinary *shaderSub)
{
	StreamDeclCacheKey key {0, layout->hash0};
	auto it = cache.insert({key, nullptr});
	PCDX11StreamDecl *&streamDecl = it.first->second;

	if (it.second) {
		D3D11_INPUT_ELEMENT_DESC *inputElementDesc = new D3D11_INPUT_ELEMENT_DESC[layout->numAttr];
		memset(inputElementDesc, 0, sizeof(D3D11_INPUT_ELEMENT_DESC) * layout->numAttr);
		MakeD3DVertexElements(inputElementDesc, layout->attrib, layout->numAttr, shaderSub->wineWorkaround);
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
	VertexDecl *layoutA,
	ShaderInputSpec *layoutB,
	bool a4,
	PCDX11ShaderBinary *shaderSub)
{
	// a4 = include normal, binormal, tangent, etc.
	Vector4 normalScaleOffset{2.0f, -1.0f, 0.0f, 0.0f};
	Vector4 texcoordScales{0.0f, 0.0f, 0.0f, 0.0f};
	Vector4 secondaryData[512 / 16];

	if (!layoutB)
		return buildStreamDecl(layoutA, shaderSub); // HACK

	StreamDeclCacheKey key {layoutB->hash0, layoutA->hash0};
	if (a4) key.hashB = ~key.hashB;
	auto it = cache.insert({key, nullptr});
	PCDX11StreamDecl *&streamDecl = it.first->second;

	if (it.second) {
		// allocate more than necessary, not all slots will be used
		D3D11_INPUT_ELEMENT_DESC *inputElementDesc = new D3D11_INPUT_ELEMENT_DESC[layoutB->numAttribs];
		memset(inputElementDesc, 0, sizeof(D3D11_INPUT_ELEMENT_DESC) * layoutB->numAttribs);

		uint32_t forbiddenBit = a4 ? 2 : 4;
		uint32_t indexB;
		uint32_t numElements = 0;
		uint32_t secondaryBufferByteOffset = 0;
		uint32_t secondaryBufferCount = 0;
		VertexAttributeB *attribB = &layoutB->attr[0];
		for (uint32_t i=0; i<layoutB->numAttribs; i++) {
			indexB = i;
			if ((attribB->field_A & forbiddenBit) == 0) {

				// printf("matching layoutB[%d] (next=%d) with\n", i, attribB->nextAttribIndex);

				// find match in layoutA, including fallbacks
				uint16_t indexA = 0xffff;
				while (indexB != -1) {
					uint32_t hash = layoutB->attr[indexB].attribKindA;
					indexA = getLayoutAIndexFromHash(layoutA, hash);

					// printf("  layoutB[%d] %08x -> layoutA[%d]\n", indexB, hash, indexA);

					if (indexA != 0xffff)
						break;

					// next attempt
					indexB = layoutB->attr[indexB].nextAttribIndex;
				}

				if (indexB != -1) {
					// input from vertex buffer

					inputElementDesc[numElements].InputSlot = 0;
					inputElementDesc[numElements].AlignedByteOffset = layoutA->attrib[indexA].offset;
					inputElementDesc[numElements].Format = MakeElementFormat(layoutA->attrib[indexA].format);

					if (attribB->attribKindB >= 10 && attribB->attribKindB < 14) {
						if (layoutA->attrib[indexA].format < 19 ||
							layoutA->attrib[indexA].format > 20
						)
							texcoordScales.vec128[attribB->attribKindB-10] = 1.0f;
						else
							texcoordScales.vec128[attribB->attribKindB-10] = 15.999512f;

					}
				} else {
					// input from secondary buffer

					inputElementDesc[numElements].AlignedByteOffset = secondaryBufferByteOffset;
					inputElementDesc[numElements].InputSlot = 1;
					inputElementDesc[numElements].InputSlotClass = D3D11_INPUT_PER_INSTANCE_DATA;
					inputElementDesc[numElements].InstanceDataStepRate = 1;
					inputElementDesc[numElements].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
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

				semanticFromEnum(inputElementDesc+numElements, attribB->attribKindB);
				// printf("    %s %d offset=%d\n",
				//	inputElementDesc[numElements].SemanticName,
				//	inputElementDesc[numElements].SemanticIndex);
				numElements++;

			} else {
				// printf("skipping layoutB[%d] (next=%d)\n", i, attribB->nextAttribIndex);
			}
			attribB++;
		}

		for (uint32_t i=0; i<numElements; i++) {
			D3D11_INPUT_ELEMENT_DESC& elem = inputElementDesc[i];
			// printf("%s[%d] at offset %d with format %d\n", elem.SemanticName, elem.SemanticIndex, elem.AlignedByteOffset, elem.Format);
		}


		streamDecl = new PCDX11StreamDecl(renderDevice, inputElementDesc, numElements, shaderSub);

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

		// HACK
		auto buffer = new PCDX11UberConstantBuffer(2);
		float rows[] = {
			normalScaleOffset.x,
			normalScaleOffset.y,
			normalScaleOffset.z,
			normalScaleOffset.w,
			texcoordScales.x,
			texcoordScales.y,
			texcoordScales.z,
			texcoordScales.w
		};
		buffer->assignRow(0, rows, 2);
		buffer->syncBuffer(deviceManager->getD3DDeviceContext());
		streamDecl->streamDeclBuffer = buffer;

		if (secondaryBufferCount) {
			auto newData = new Vector4[secondaryBufferCount];
			streamDecl->secondaryData = newData;
			memcpy(newData, secondaryData, secondaryBufferCount * 4);
			streamDecl->vertexBuffer = new PCDX11SimpleStaticVertexBuffer(
				16 * secondaryBufferCount, 1, newData);
		}
	}

	return streamDecl;
}

}
