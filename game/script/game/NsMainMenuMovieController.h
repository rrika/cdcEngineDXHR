#pragma once
#include <cstdint>

struct GFxValue {
	void *objectInterface;
	enum ValueType {
		VT_Boolean = 2,
		VT_Number = 3,
		VT_String = 4,
	} type;
	union {
		double number;
		bool boolean;
		const char *string;
	};
};

class NsMainMenuMovieController {
public:
	uint32_t ProcessMovieFunction(const char *name, GFxValue *args, uint32_t numArgs);
};
