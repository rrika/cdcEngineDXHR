#pragma once
#include "PCDX11Shader.h"
#include "PCDX11DeviceManager.h"
#include "PCDX11ShaderManager.h"
#include "PCDX11PixelShader.h"
#include "PCDX11VertexShader.h"
// #include "PCDX11HullShader.h"
// #include "PCDX11DomainShader.h"
// #include "PCDX11ComputeShader.h"


namespace cdc {

class PCDX11ShaderTable {
protected:
	char *blob;
	uint32_t *offsets = nullptr;
	uint32_t numShaders = 0;
public:
	PCDX11ShaderTable(char *blob) : blob(blob) {}
};

class PCDX11VertexShaderTable : public PCDX11ShaderTable {
	// uint32_t dword10;
	PCDX11VertexShader **vertexShaders = nullptr;
	bool hasOwnership = 0;
	// uint8_t byte19;
	// uint8_t byte1A;
	// uint8_t byte1B;
public:
	PCDX11VertexShaderTable(char *blob, bool takeCopy) :
		PCDX11ShaderTable(blob)
	{
		auto *blobWords = (uint32_t*)blob;
		offsets = &blobWords[2];
		numShaders = blobWords[0];
		vertexShaders = new PCDX11VertexShader*[numShaders];
		hasOwnership = takeCopy;
		memset(vertexShaders, 0, sizeof(PCDX11VertexShader*[numShaders]));
		for (uint32_t i = 0; i < numShaders; i++)
			vertexShaders[i] = deviceManager->getShaderManager()->createVertexShader(
				/*blob=*/ blob + offsets[i],
				/*takeCopy=*/ hasOwnership,
				/*isWrapped=*/ true);
	}
};

class PCDX11PixelShaderTable : public PCDX11ShaderTable {
	// uint32_t dword10;
	PCDX11PixelShader **pixelShaders = nullptr;
	bool hasOwnership = false;
	// uint8_t f19[3];
	// uint32_t dword1C;
public:
	PCDX11PixelShaderTable(char *blob, bool takeCopy) :
		PCDX11ShaderTable(blob)
	{
		// auto *blobWords = (uint32_t*)blob;
		// offsets = &blobWords[2];
		// numShaders = blobWords[0];
		// vertexShaders = new PCDX11PixelShader*[numShaders];
		// hasOwnership = takeCopy;
		// memset(vertexShaders, 0, sizeof(PCDX11VertexShader*[numShaders]));
		// for (uint32_t i = 0; i < numShaders; i++)
		// 	vertexShaders[i] = deviceManager->getShaderManager()->createPixelShader(
		// 		/*blob=*/ blob + offsets[i],
		// 		/*takeCopy=*/ hasOwnership,
		// 		/*isWrapped=*/ true);
	}
};

/*
class PCDX11HullShaderTable : public PCDX11ShaderTable {
public:
	// todo
};

class PCDX11DomainShaderTable : public PCDX11ShaderTable {
public:
	// todo
};

class PCDX11ComputeShaderTable : public PCDX11ShaderTable {
public:
	// todo
};
*/

}
