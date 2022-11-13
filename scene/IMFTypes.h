#pragma once
#include <cstdint>

namespace dtp { struct IntermediateMesh; }

namespace cdc {

dtp::IntermediateMesh *GetIMFPointerFromId(uint32_t);

}
