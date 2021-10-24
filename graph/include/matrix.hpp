#pragma once

#include "stdafx.h"
#include "vector.hpp"

namespace linalg {

template <typename T>
class Matrix;

template <typename T>
Matrix<T> multiplyStrassen(const Matrix<T>& a, const Matrix<T>& b,
                           size_t baseBlockSize);

template <typename T>
Matrix<T> operator*(const Matrix<T>& a, const Matrix<T>& b);

class SizeMismatchException : public std::exception {};

template <typename ValueT>
class Matrix {
 private:
  using Container = std::vector<ValueT>;

 public:
  using value_type = typename Container::value_type;
  using reference = value_type&;
  using const_reference = const value_type&;
  using pointer = typename Container::pointer;
  using const_pointer = typename Container::const_pointer;

  Matrix(size_t rows, size_t columns, value_type fill_value)
      : rows(rows), columns(columns), elements(rows * columns, fill_value) {}

  Matrix(size_t rows, size_t columns) : Matrix(rows, columns, value_type()) {}

  explicit Matrix(size_t size) : Matrix(size, size) {}

  Matrix(const Matrix& copy) = default;

  Matrix(Matrix&&) = default;

  Matrix& operator=(const Matrix& a) = default;

  virtual ~Matrix() = default;

  reference at(size_t row, size_t col) {
    return this->elements.at(getLinearIndex(row, col));
  }

  const_reference at(size_t row, size_t col) const {
    return this->elements.at(getLinearIndex(row, col));
  }

  const Matrix& operator*=(const Matrix& a) {
    if (columnCount() != a.rowCount()) {
      throw SizeMismatchException();
    }

    Matrix temp(*this);

    this->resize(rows, a.columns);
    fill(static_cast<value_type>(0));

#pragma omp parallel
#pragma omp for simd collapse(3)
    for (size_t i = 0; i < temp.columnCount(); ++i) {
      for (size_t row = 0; row < rowCount(); ++row) {
        for (size_t col = 0; col < a.columnCount(); ++col) {
          this->at(row, col) += temp.at(row, i) * a.at(i, col);
        }
      }
    }

    return *this;
  }

  const Matrix& operator+=(const Matrix& a) {
    checkShapeEquality(a);

#pragma omp parallel for simd
    for (size_t i = 0; i < elements.size(); ++i) {
      elements[i] += a.elements[i];
    }

    return *this;
  }

  Matrix operator-=(const Matrix& a) {
    checkShapeEquality(a);

#pragma omp parallel for simd
    for (size_t i = 0; i < elements.size(); ++i) {
      elements[i] -= a.elements[i];
    }

    return *this;
  }

  Matrix operator+(const Matrix& other) const {
    Matrix sum(*this);
    return sum += other;
  }

  Matrix operator-(const Matrix& other) const {
    Matrix diff(*this);
    return diff -= other;
  }

  bool operator==(const Matrix& a) const {
    if (rows != a.rows || columns != a.columns) {
      return false;
    }

    for (size_t i{}; i < elements.size(); ++i) {
      if (elements.at(i) != a.elements.at(i)) {
        return false;
      }
    }

    return true;
  }

  bool operator!=(const Matrix& a) const { return !(*this == a); }

  void fill(const_reference fill_value) {
    for (size_t i{}; i < elements.size(); ++i) {
      elements[i] = fill_value;
    }
  }

  void randomFill(const_reference min_value, const_reference max_value,
                  std::default_random_engine& engine) {
    if constexpr (std::is_floating_point<value_type>::value) {
      std::uniform_real_distribution<value_type> random_gen(min_value,
                                                            max_value);
      random_fill_with_gen(random_gen, engine);
    } else {
      std::uniform_int_distribution<value_type> random_gen(min_value,
                                                           max_value);

      random_fill_with_gen(random_gen, engine);
    }
  }

  Matrix transpose() const {
    Matrix transposed(columnCount(), rowCount());

    for (size_t row{}; row < rowCount(); ++row) {
      for (size_t col{}; col < columnCount(); ++col) {
        transposed.at(col, row) = this->at(row, col);
      }
    }

    return transposed;
  }

  size_t rowCount() const { return rows; }

  size_t columnCount() const { return columns; }

  const_pointer getRawData() const { return elements.data(); }

  pointer getRawData() { return elements.data(); }

  Matrix powerOf(uint power) const {
    using std::function;
    using std::placeholders::_1;
    using std::placeholders::_2;

    Matrix powerMatrix{*this};

    std::function<Matrix(const Matrix&, const Matrix&)>
        mult_oper{operator*<value_type> };

    if (this->rowCount() == this->columnCount()) {
      mult_oper = std::bind(multiplyStrassen<ValueT>, _1, _2, 16);
    }

    std::stack<Matrix> reminders;

    while (power > 1) {
      if (power % 2 == 1) {
        reminders.push(powerMatrix);
      }
      powerMatrix = mult_oper(powerMatrix, powerMatrix);

      power /= 2;
    }

    while (!reminders.empty()) {
      powerMatrix *= reminders.top();
      reminders.pop();
    }

    return powerMatrix;
  }

 private:
  template <typename RandomGen>
  void random_fill_with_gen(RandomGen& random_gen,
                            std::default_random_engine& engine) {
    for (size_t i{}; i < elements.size(); ++i) {
      elements[i] = random_gen(engine);
    }
  }

  void checkShapeEquality(const Matrix& other) const {
    if (rows != other.rows || columns != other.columns) {
      throw SizeMismatchException();
    }
  }

  size_t getLinearIndex(size_t row, size_t column) const {
    return row * columnCount() + column;
  }

  void resize(size_t new_rows, size_t new_cols) {
    if (new_rows == rowCount() && new_cols == columnCount()) {
      return;
    }

    elements.resize(new_rows * new_cols);
    rows = new_rows;
    columns = new_cols;
  }

  Container elements;
  size_t rows, columns;
};

template <typename T>
Matrix<T> operator*(const Matrix<T>& a, const Matrix<T>& b) {
  auto matrix_mult(a);

  return matrix_mult *= b;
}

template <typename T>
Matrix<T> operator*(const Matrix<T>& a, const T& value) {
  auto matrix_mult(a);

  for (size_t row{}; row < matrix_mult.rowCount(); ++row) {
    for (size_t col{}; col < matrix_mult.columnCount(); ++col) {
      matrix_mult.at(row, col) *= value;
    }
  }

  return matrix_mult;
}

template <typename T>
Matrix<T> operator*(const T& value, const Matrix<T>& a) {
  return a * value;
}

template <typename T>
std::ostream& operator<<(std::ostream& output, const Matrix<T>& matrix) {
  for (size_t row{}; row < matrix.rowCount(); ++row) {
    size_t column{};
    for (; column < matrix.columnCount() - 1; ++column) {
      output << matrix.at(row, column) << ' ';
    }

    output << matrix.at(row, column) << '\n';
  }

  return output;
}

template <typename T>
Matrix<T> multiplyStrassenR(const Matrix<T>& a, const Matrix<T>& b,
                            size_t baseBlockSize) {
  if (a.rowCount() <= baseBlockSize) {
    return a * b;
  }

  size_t newBlockSize = a.rowCount() / 2;

  Matrix<T> a11(newBlockSize), a12(newBlockSize), a21(newBlockSize),
      a22(newBlockSize), b11(newBlockSize), b12(newBlockSize),
      b21(newBlockSize), b22(newBlockSize);

  for (size_t row{}; row < newBlockSize; ++row) {
    for (size_t col{}; col < newBlockSize; ++col) {
      a11.at(row, col) = a.at(row, col);
      a12.at(row, col) = a.at(row, col + newBlockSize);
      a21.at(row, col) = a.at(row + newBlockSize, col);
      a22.at(row, col) = a.at(row + newBlockSize, col + newBlockSize);

      b11.at(row, col) = b.at(row, col);
      b12.at(row, col) = b.at(row, col + newBlockSize);
      b21.at(row, col) = b.at(row + newBlockSize, col);
      b22.at(row, col) = b.at(row + newBlockSize, col + newBlockSize);
    }
  }

  Matrix<T> p1 = multiplyStrassenR(a11 + a22, b11 + b22, baseBlockSize);
  Matrix<T> p2 = multiplyStrassenR(a21 + a22, b11, baseBlockSize);
  Matrix<T> p3 = multiplyStrassenR(a11, b12 - b22, baseBlockSize);
  Matrix<T> p4 = multiplyStrassenR(a22, b21 - b11, baseBlockSize);
  Matrix<T> p5 = multiplyStrassenR(a11 + a12, b22, baseBlockSize);
  Matrix<T> p6 = multiplyStrassenR(a21 - a11, b11 + b12, baseBlockSize);
  Matrix<T> p7 = multiplyStrassenR(a12 - a22, b21 + b22, baseBlockSize);

  Matrix<T> c11{p1 + p4 - p5 + p7};
  Matrix<T> c12{p3 + p5};
  Matrix<T> c21{p2 + p4};
  Matrix<T> c22{p1 - p2 + p3 + p6};

  Matrix<T> c(a.rowCount());

  for (size_t row{}; row < newBlockSize; ++row) {
    for (size_t col{}; col < newBlockSize; ++col) {
      c.at(row, col) = c11.at(row, col);
      c.at(row, col + newBlockSize) = c12.at(row, col);
      c.at(row + newBlockSize, col) = c21.at(row, col);
      c.at(row + newBlockSize, col + newBlockSize) = c22.at(row, col);
    }
  }

  return c;
}

template <typename T>
Matrix<T> multiplyStrassen(const Matrix<T>& a, const Matrix<T>& b,
                           size_t baseBlockSize) {
  if (a.rowCount() <= baseBlockSize) {
    return a * b;
  }

  size_t newBlockSize = a.rowCount() / 2;

  Matrix<T> a11(newBlockSize), a12(newBlockSize), a21(newBlockSize),
      a22(newBlockSize), b11(newBlockSize), b12(newBlockSize),
      b21(newBlockSize), b22(newBlockSize);

  for (size_t row{}; row < newBlockSize; ++row) {
    for (size_t col{}; col < newBlockSize; ++col) {
      a11.at(row, col) = a.at(row, col);
      a12.at(row, col) = a.at(row, col + newBlockSize);
      a21.at(row, col) = a.at(row + newBlockSize, col);
      a22.at(row, col) = a.at(row + newBlockSize, col + newBlockSize);

      b11.at(row, col) = b.at(row, col);
      b12.at(row, col) = b.at(row, col + newBlockSize);
      b21.at(row, col) = b.at(row + newBlockSize, col);
      b22.at(row, col) = b.at(row + newBlockSize, col + newBlockSize);
    }
  }

  Matrix<T> p1 = multiplyStrassenR(a11 + a22, b11 + b22, baseBlockSize);
  Matrix<T> p2 = multiplyStrassenR(a21 + a22, b11, baseBlockSize);
  Matrix<T> p3 = multiplyStrassenR(a11, b12 - b22, baseBlockSize);
  Matrix<T> p4 = multiplyStrassenR(a22, b21 - b11, baseBlockSize);
  Matrix<T> p5 = multiplyStrassenR(a11 + a12, b22, baseBlockSize);
  Matrix<T> p6 = multiplyStrassenR(a21 - a11, b11 + b12, baseBlockSize);
  Matrix<T> p7 = multiplyStrassenR(a12 - a22, b21 + b22, baseBlockSize);

  Matrix<T> c11{p1 + p4 - p5 + p7};
  Matrix<T> c12{p3 + p5};
  Matrix<T> c21{p2 + p4};
  Matrix<T> c22{p1 - p2 + p3 + p6};

  Matrix<T> c(a.rowCount());

  for (size_t row{}; row < newBlockSize; ++row) {
    for (size_t col{}; col < newBlockSize; ++col) {
      c.at(row, col) = c11.at(row, col);
      c.at(row, col + newBlockSize) = c12.at(row, col);
      c.at(row + newBlockSize, col) = c21.at(row, col);
      c.at(row + newBlockSize, col + newBlockSize) = c22.at(row, col);
    }
  }

  return c;
}

}  // namespace linalg
