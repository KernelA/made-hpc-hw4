#pragma once

#include "matrix.hpp"

namespace linalg {

template <typename ValueT>
class Matrix;

template <typename ValueT>
class Vector : public Matrix<ValueT> {
 public:
  Vector(size_t size) : Matrix<ValueT>(size, 1) {}

  Vector(const Matrix<ValueT>& matrix) : Matrix<ValueT>(matrix) {
    if (matrix.columnCount() != 1) {
      throw std::invalid_argument(
          "Expected matrix with number of columns equal to 1");
    }
  }

  Vector(const Vector&) = default;
  ~Vector() = default;

  template <
      typename NormType = ValueT,
      std::enable_if_t<std::is_floating_point<NormType>::value, bool> = true>
  void normalizeL2() {
    NormType norm{std::sqrt(this->squaredNormL2())};

    for (size_t row{}; row < this->size(); ++row) {
      this->at(row) /= norm;
    }
  }

  Vector operator*(const linalg::Vector<ValueT>& vector) {
    return *this * static_cast<const linalg::Matrix<ValueT>&>(vector);
  }

  ValueT squaredNormL2() const {
    ValueT norm{};

    for (size_t index{}; index < this->size(); ++index) {
      norm += this->at(index) * this->at(index);
    }

    return norm;
  }

  typename Matrix<ValueT>::reference at(size_t index) {
    return static_cast<Matrix<ValueT>*>(this)->at(index, 0);
  }

  typename Matrix<ValueT>::const_reference at(size_t index) const {
    return static_cast<const Matrix<ValueT>*>(this)->at(index, 0);
  }

  size_t size() const { return this->rowCount(); }
};

};  // namespace linalg
