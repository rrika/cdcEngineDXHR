#include "Assert.h"
#include <cstdio>
#include <cstdarg>

namespace cdc {

void FatalError(const char *format, ...) {
	va_list args;
	va_start(args, format);
	vfprintf(stderr, format, args);
	va_end(args);
	fputs("", stderr); // newline
}

}
