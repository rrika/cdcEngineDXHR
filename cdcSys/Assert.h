#pragma once

#define CDC_Assert(expr) { if (!(expr)) FatalError("Assertion failure: " #expr); }

namespace cdc {

void FatalError(const char *format, ...);

}
