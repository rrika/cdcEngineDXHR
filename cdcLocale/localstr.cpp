#include <cstdio>
#include "localstr.h"
#include "cdcFile/FileHelpers.h"
#include "cdcResource/Specialisation.h"

struct LocalizationTable {
	uint32_t languageCode;
	uint32_t textLanguageCode;
	bool loaded;
	char *locals_bin;
	const char **strings;
};

LocalizationTable ltable = {0xffffffff, 0xffffffff, false, nullptr, nullptr};
extern char pathPrefix[36];

language_t localstr_get_text_language() {
	// unconfirmed
	return (language_t)ltable.textLanguageCode;
}

language_t localstr_get_voice_language() {
	// unconfirmed
	return (language_t)ltable.languageCode;
}

void localstr_set_language(language_t voLang, language_t textLang) {
	// HACK
	if (voLang == language_default)
		voLang = language_english;

	if (textLang == language_default)
		textLang = voLang;

	ltable.languageCode = voLang;
	ltable.textLanguageCode = textLang;
	Specialisation::BlockingChange(1<<textLang);
	if (!ltable.locals_bin)
		localstr_reload();
}

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

uint32_t localstr_get_count() {
	uint32_t numStrings = *(uint32_t*)(ltable.locals_bin+4);
	return numStrings;
}
