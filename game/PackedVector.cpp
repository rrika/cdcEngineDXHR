#include "PackedVector.h"

cdc::Vector PackedBytesAndScale::unpack() {
	return {
		((uint8_t)(bytes >>  0)) * scale / 255.0f,
		((uint8_t)(bytes >>  8)) * scale / 255.0f,
		((uint8_t)(bytes >> 16)) * scale / 255.0f,
		((uint8_t)(bytes >> 24)) * scale / 255.0f
	};
}
