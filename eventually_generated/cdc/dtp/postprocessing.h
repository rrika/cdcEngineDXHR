#pragma once
#include <cstdint>

// name of file and contained structs is guessed

namespace cdc { class IMaterial; }

namespace dtp {

struct PPVariableBlob {
	const char *name; // 0
	uint32_t format; // 4
	union { // 8
		uint32_t packed;
		float x;
	};
	float y; // C
	float z; // 10
	float w; // 14
};

struct PPTextureBlob {
	const char *name;
	uint32_t dword4;
	uint32_t dword8;
	uint32_t width;
	uint32_t height;
	uint32_t dword14;
	uint8_t byte18;
};

struct PPPrePassBlob {
	const char *name;
	uint32_t outputTextureIndex; // 4
	uint32_t clearMode;
	union {
		uint32_t clearColor;
		uint32_t sourceTextureIndex;
	};
	float clearColorScale;
	bool enabled; // 14
	bool useDepthBuffer;
	bool byte16;
	uint32_t variableIndices[24]; // 18
	uint32_t textureIndices[8]; // 78
	cdc::IMaterial *material; // 98
	uint32_t builtinShaderType; // 9C
};

struct PPPassBlob {
	const char *name;
	uint32_t numPrePasses; // 4
	PPPrePassBlob *prePasses; // 8
	bool enabled; // C
	bool useDepthBuffer; // D
	bool byteE; // clearMode
	bool gatePrePasses; // F, prepasses only run when main pass runs
	bool blendOverPrevious; // 10
	uint32_t variableIndices[24]; // 14
	uint32_t textureIndices[8]; // 74
	cdc::IMaterial *material; // 94
	uint8_t scaleform; // 98
	uint8_t canBeDisabledThroughSettings; // 99
	uint32_t builtinShaderType; // 9C
};

struct PPVariableBlobArray {
	uint32_t size; // 0
	PPVariableBlob *data; // 4
};

struct PPPassBlobArray {
	uint32_t size; // 0
	PPPassBlob *data; // 4
};

struct PPTextureBlobArray {
	uint32_t size; // 0
	PPTextureBlob *data; // 4
};

// dtp path postprocess/effects/dx3_postprocess_v2.ppfx
struct PPVarPassTexBlobs {
	PPVariableBlobArray variables; // 0
	PPPassBlobArray passes; // 8
	PPTextureBlobArray textures; // 10
};

struct PPVariableUnk2Link {
	uint32_t variableIndex;
	uint32_t type4;
	uint32_t dword8;
	float floatC;
	float float10;
	float float14;
	float float18;
	uint32_t blendMode1C;
};

struct PPDefaultInputs;

// eg. dtp path postprocess/datas/cinefx.ppdat
struct PPActiveSet {
	char *name; // 0
	PPVarPassTexBlobs *varPassTex; // 4
	uint32_t numLinks; // 8
	PPVariableUnk2Link *links; // C

	uint32_t alwaysActivePassCount; // 10
	uint32_t *alwaysActivePassIndices; // 14

	uint32_t dword18;
	void *dword1C;

	uint32_t dword20;
	void *dword24;

	uint32_t dof1PassCount; // 28
	uint32_t *dof1PassIndices; // 2C

	uint32_t dof2PassCount; // 30
	uint32_t *dof2PassIndices; // 34

	uint32_t antialiasPassCount; // 38
	uint32_t *antialiasPassIndices; // 3C

	uint32_t antialias2PassCount; // 40
	uint32_t *antialias2PassIndices; // 44

	uint32_t neverActivePassCount; // 48
	uint32_t *neverActivePassIndices; // 4C

	uint32_t dword50;
	float float54;
};

}
