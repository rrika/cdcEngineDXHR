#pragma once
#include <cstdint>
#include "../sys/Array.h"

class PPTexture;
class PPVariable;
struct PPUnknown3;
struct PPVariableUnk2Link;

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

struct PPUnknown1 { // aka PPActiveSet
	char *name;
	PPUnknown3 *unknown3;
	uint32_t numLinks8;
	PPVariableUnk2Link *linksC;

	uint32_t alwaysActivePassCount;
	uint32_t *alwaysActivePassIndices;

	uint32_t dword18;
	void *dword1C;

	uint32_t dword20;
	void *dword24;

	uint32_t dof1PassCount;
	uint32_t *dof1PassIndices;

	uint32_t dof2PassCount;
	uint32_t *dof2PassIndices;

	uint32_t antialiasPassCount;
	uint32_t *antialiasPassIndices;

	uint32_t antialias2PassCount;
	uint32_t *antialias2PassIndices;

	uint32_t neverActivePassCount;
	uint32_t *neverActivePassIndices;

	uint32_t dword50;
	float float54;
};

struct PPUnknown2 {
	uint32_t dword0;
	uint32_t dword4;
	uint32_t dword8;
	uint32_t dwordC;
	uint32_t dword10;
	uint32_t dword14;
	uint32_t dword18;
	uint32_t dword1C;
	uint32_t dword20;
	uint32_t dword24;
	uint32_t dword28;
	uint32_t dword2C;
};

class PPManager {
public:
	Array<PPUnknown1*> unknowns1; // 4
	Array<PPTexture*> textures; // 10
	Array<PPVariable*> variables; // 1C
	uint32_t rootPasses28;
	uint32_t dwords2C[32];
	uint32_t dwordsAC[32];
	uint32_t dwords12C[32];
	uint32_t width;
	uint32_t height;
	Array<PPUnknown2*> unknowns2; // 1B4

	PPManager();
	virtual ~PPManager() = default;

	void prepare();
	void addUnknown1(PPUnknown1 *unk1, float f);
};
