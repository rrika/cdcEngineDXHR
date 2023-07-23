#pragma once
#include <cstdint>

namespace dtp { struct Model; }

namespace cdc {

struct Matrix;
class IMatrixState;

void CalcSkeletonMatrices(dtp::Model *model, Matrix *src, uint32_t boneCount, IMatrixState *dst);

}