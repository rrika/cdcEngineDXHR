#pragma once
#include "../PCDeviceManager.h"
#include "PCPixelShader.h"
#include "PCShaderBinary.h"
#include "PCShaderManager.h"
#include "PCVertexShader.h"

namespace cdc {

class PCShaderTable {
protected:
	char *blob;
	uint32_t *offsets = nullptr;
	uint32_t numShaders = 0;
public:
	PCShaderTable(char *blob) : blob(blob) {}
};

class PCVertexShaderTable : public PCShaderTable {
public:
	// uint32_t dword10;
	PCVertexShader **vertexShaders = nullptr; // 14
	bool hasOwnership = 0; // 18
	// uint8_t byte19;
	// uint8_t byte1A;
	// uint8_t byte1B;
public:
	PCVertexShaderTable(char *blob, bool takeCopy) :
		PCShaderTable(blob)
	{
		auto *blobWords = (uint32_t*)blob;
		offsets = &blobWords[2];
		numShaders = blobWords[0] >> 2;
		vertexShaders = new PCVertexShader*[numShaders];
		hasOwnership = takeCopy;
		memset(vertexShaders, 0, sizeof(PCVertexShader*) * numShaders);
		for (uint32_t i = 0; i < numShaders; i++)
			if (offsets[i] != ~0u) {
				vertexShaders[i] = deviceManager9->getShaderManager()->createVertexShader(
					/*blob=*/ blob + offsets[i],
					/*takeCopy=*/ hasOwnership,
					/*isWrapped=*/ true);
			} else
				vertexShaders[i] = nullptr;

		// if (!neverPrecreate ...) {
		//     // TODO
		// }

		if (takeCopy) {
			blob = nullptr;
			offsets = 0;
		}
	}

	PCVertexShader *operator[](size_t i) const { return vertexShaders[i]; }
};

class PCPixelShaderTable : public PCShaderTable {
public:
	// uint32_t dword10;
	PCPixelShader **pixelShaders = nullptr; // 14
	bool hasOwnership = false; // 18
	// uint8_t f19[3];
	// uint32_t dword1C;
public:
	PCPixelShaderTable(char *blob, bool takeCopy) :
		PCShaderTable(blob)
	{
		auto *blobWords = (uint32_t*)blob;
		offsets = &blobWords[2];
		numShaders = blobWords[0] >> 2;
		pixelShaders = new PCPixelShader*[numShaders];
		hasOwnership = takeCopy;
		memset(pixelShaders, 0, sizeof(PCPixelShader*) * numShaders);
		for (uint32_t i = 0; i < numShaders; i++)
			if (offsets[i] != ~0u)
				*(uint32_t*)(blob + offsets[i]) ^= (i&1); // what??

		for (uint32_t i = 0; i < numShaders; i++)
			if (offsets[i] != ~0u)
				pixelShaders[i] = deviceManager9->getShaderManager()->createPixelShader(
					/*blob=*/ blob + offsets[i],
					/*takeCopy=*/ hasOwnership,
					/*isWrapped=*/ true);

		// if (!neverPrecreate ...) {
		//     // TODO
		// }

		if (takeCopy) {
			blob = nullptr;
			offsets = 0;
		}
	}

	PCPixelShader *operator[](size_t i) const { return pixelShaders[i]; }
};

}
