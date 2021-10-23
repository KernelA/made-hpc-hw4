#pragma once

#include "matrix.hpp"

namespace linalg {

template <typename ValueT>
class Vector : public Matrix<ValueT> {
 public:
  Vector(size_t size) : Matrix(size, 1) {}
  Vector(const Vector&) = default;
  ~Vector() = default;
};

};  // namespace linalg
