#pragma once
#include <cstdint>
#include "cdcMath/Math.h"
#include "cdc/dtp/postprocessing.h"

class PPVariable {
public:
	uint32_t dword4;
	uint32_t dword8;
	uint32_t dwordC;
	cdc::Vector m_value = {0.0f, 0.0f, 0.0f, 0.0f};
	dtp::PPVariableBlob *m_blob = nullptr;
	uint32_t dword24;
	uint32_t dword28;
	uint32_t dword2C;

	void init(dtp::PPVariableBlob *blob);

	virtual ~PPVariable() = default;
};
