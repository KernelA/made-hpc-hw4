#pragma once

#include "matrix.h"

namespace linalg {

class Vector : public Matrix {
 public:
  Vector(size_t size);
  Vector(const Vector&) = default;
  ~Vector() = default;
};

};  // namespace linalg
