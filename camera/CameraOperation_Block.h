#pragma once
#include "CameraOperation.h"

template <int count>
class CameraOperation_Block : public CameraOperation {
public:
	CameraOperation *data[count];
};
