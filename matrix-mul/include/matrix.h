#pragma once

#include "stdafx.h"

namespace linalg {

const double EPS = 1e-6;

class SizeMismatchException : public std::exception {};

class Matrix {
 public:
  Matrix(size_t rows, size_t cols);

  Matrix(size_t size);

  Matrix(const Matrix& copy) = default;

  Matrix& operator=(const Matrix& a) = default;

  double& get(size_t row, size_t col);

  const double& get(size_t row, size_t col) const;

  void set(size_t row, size_t col, const double& value);

  Matrix& operator*=(const Matrix& a);

  Matrix operator*(const Matrix& a) const;

  Matrix& operator+=(const Matrix& a);

  Matrix operator-(const Matrix& other) const;

  Matrix operator+(const Matrix& other) const;

  Matrix& operator-=(const Matrix& a);

  bool operator==(const Matrix& a) const;
  bool operator!=(const Matrix& a) const;

  void fill(double fill_value);

  ~Matrix() = default;

  size_t rowCount() const;

  size_t columnCount() const;

  const double* getRawData() const;

  double* getRawData();

  friend std::istream& operator>>(std::istream& input, Matrix& matrix);

 private:
  void resize(size_t rows, size_t columns);

  void copyElements(const Matrix& other);

  void checkShapeEquality(const Matrix& other) const;

  size_t getLinearIndex(size_t row, size_t column) const;

  std::vector<double> elements;
  size_t rows, columns;
};

std::ostream& operator<<(std::ostream& output, const Matrix& matrix);

std::istream& operator>>(std::istream& input, Matrix& matrix);

Matrix multiplyStrassen(const Matrix& a, const Matrix& b, size_t baseBlockSize);

Matrix multiplyStrassenR(const Matrix& a, const Matrix& b,
                         size_t baseBlockSize);

}  // namespace linalg