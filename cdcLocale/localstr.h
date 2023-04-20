#pragma once
#include <cstdint>

enum language_t {
	language_default = -1,
	language_english = 0,
	language_french = 1,
	language_german = 2,
	language_italian = 3,
	language_spanish = 4,
	language_japanese = 5,
	language_portuguese = 6,
	language_polish = 7,
	language_ukenglish = 8,
	language_russian = 9,
	language_czech = 10,
	language_dutch = 11,
	language_hungarian = 12,
	language_num_states = 13
};

language_t localstr_get_text_language();
language_t localstr_get_voice_language();
void localstr_set_language(language_t, language_t);
void localstr_reload();
const char *localstr_get(uint32_t);
