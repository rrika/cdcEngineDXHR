#include "Assert.h"
#include <cstdio>
#include <cstdarg>
#ifdef __linux__
#include <csignal>
#endif

namespace cdc {

void FatalError(const char *format, ...) {
	#ifdef __linux__
		fputs("\033[31m", stderr); // set RED color
	#endif
	va_list args;
	va_start(args, format);
	vfprintf(stderr, format, args);
	va_end(args);
	#ifdef __linux__
		fputs("\033[0m\n", stderr); // reset color + newline
		raise(SIGTRAP);
	#else
		fputs("\n", stderr); // newline
	#endif
}

}

void todo() {
	cdc::FatalError("todo");
}
