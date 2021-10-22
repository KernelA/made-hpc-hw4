#pragma once

#include "matrix.h"
#include "stdafx.h"

namespace linalg {

void fill_matrix(Matrix& a, double min_value = -100, double max_value = 100);

size_t nextPowerOfTwo(size_t n);

}  // namespace linalg
