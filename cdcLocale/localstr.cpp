#include <cstdio>
#include "localstr.h"
#include "cdcFile/FileHelpers.h"

struct LocalizationTable {
	uint32_t languageCode;
	uint32_t textLanguageCode;
	bool loaded;
	char *locals_bin;
	const char **strings;
};

LocalizationTable ltable = {0xffffffff, 0xffffffff, false, nullptr, nullptr};
extern char pathPrefix[36];

void localstr_reload() {
	if (ltable.loaded)
		return;
	char path[256];
	sprintf(path, "%slocal\\locals.bin", pathPrefix);
	if (ltable.locals_bin)
		delete[] ltable.locals_bin;
	printf("[%s]\n", path);
	ltable.locals_bin = cdc::FSHelper_ReadFile(path);
	ltable.strings = (const char**)(ltable.locals_bin + 8);
	uint32_t numStrings = *(uint32_t*)(ltable.locals_bin+4);
	uint32_t *offsets = (uint32_t*)ltable.strings;
	for (uint32_t i = 0; i<numStrings; i++) {
		if (offsets[i])
			offsets[i] += (uint32_t)ltable.locals_bin;
	}
}

const char *localstr_get(uint32_t index) {
	uint32_t numStrings = *(uint32_t*)(ltable.locals_bin+4);
	if (ltable.strings && ltable.locals_bin &&
		index >= 0 && index < numStrings
	)
		return ltable.strings[index];
	else
		return "";
}
