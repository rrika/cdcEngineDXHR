#include "PPVariable.h"

void PPVariable::init(dtp::PPVariableBlob *blob) {
	m_blob = blob;
	cdc::Vector value = {0.0f, 0.0f, 0.0f, 0.0f};
	switch (blob->format) {
	case 3:
	case 7:
	case 11:
		value.w = blob->w;
	case 2:
	case 6:
	case 10:
		value.z = blob->z;
	case 1:
	case 5:
	case 9:
		value.y = blob->y;
	case 0:
	case 4:
	case 8:
		value.x = blob->x;
		break;
	case 12:
		value.x = ((blob->packed >>  0) & 0xff) * blob->y / 255.0f;
		value.y = ((blob->packed >>  8) & 0xff) * blob->y / 255.0f;
		value.z = ((blob->packed >> 16) & 0xff) * blob->y / 255.0f;
		value.w = ((blob->packed >> 24) & 0xff) * blob->y / 255.0f;
	}
	m_value = value;
}
