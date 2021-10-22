#include "matrix.h"

#include "utils.h"

using namespace linalg;

void Matrix::fill(double fill_value) {
  for (size_t i{}; i < elements.size(); ++i) {
    elements[i] = fill_value;
  }
}

Matrix::Matrix(size_t rows, size_t columns)
    : rows(rows), columns(columns), elements(rows * columns, 0.0) {}

Matrix::Matrix(size_t size) : Matrix(size, size) {}

double& Matrix::get(size_t row, size_t col) {
  return this->elements.at(getLinearIndex(row, col));
}

size_t Matrix::getLinearIndex(size_t row, size_t column) const {
  return row * columnCount() + column;
}

const double& Matrix::get(size_t row, size_t col) const {
  return this->elements.at(getLinearIndex(row, col));
}

void Matrix::set(size_t row, size_t col, const double& value) {
  this->elements.at(getLinearIndex(row, col)) = value;
}

void Matrix::checkShapeEquality(const Matrix& other) const {
  if (rows != other.rows || columns != other.columns) {
    throw SizeMismatchException();
  }
}

Matrix& Matrix::operator*=(const Matrix& a) {
  if (columns != a.rows) {
    throw SizeMismatchException();
  }

  Matrix temp(*this);

  this->resize(rows, a.columns);
  fill(0.0);

  for (size_t i{}; i < temp.columnCount(); ++i) {
    for (size_t row{}; row < rowCount(); ++row) {
      for (size_t col{}; col < a.columnCount(); ++col) {
        this->get(row, col) += temp.get(row, i) * a.get(i, col);
      }
    }
  }

  return *this;
}

Matrix& Matrix::operator+=(const Matrix& a) {
  checkShapeEquality(a);

  for (size_t i{}; i < elements.size(); ++i) {
    elements[i] += a.elements[i];
  }

  return *this;
}

Matrix& Matrix::operator-=(const Matrix& a) {
  checkShapeEquality(a);

  for (size_t i{}; i < elements.size(); ++i) {
    elements[i] -= a.elements[i];
  }

  return *this;
}

Matrix Matrix::operator+(const Matrix& other) const {
  Matrix sum(*this);
  return sum += other;
}

Matrix Matrix::operator-(const Matrix& other) const {
  Matrix sum(*this);
  return sum -= other;
}

Matrix Matrix::operator*(const Matrix& a) const {
  Matrix matrix_mult(*this);

  return matrix_mult *= a;
}

void Matrix::resize(size_t new_rows, size_t new_cols) {
  if (new_rows == rowCount() && new_cols == columnCount()) {
    return;
  }

  elements.resize(new_rows * new_cols);
  rows = new_rows;
  columns = new_cols;
}

bool Matrix::operator==(const Matrix& a) const {
  if (rows != a.rows || columns != a.columns) {
    return false;
  }

  for (size_t i{}; i < rows; ++i) {
    for (size_t j{}; j < columns; ++j) {
      if (std::abs(this->get(i, j) - a.get(i, j)) > EPS) {
        return false;
      }
    }
  }

  return true;
}

bool Matrix::operator!=(const Matrix& a) const { return !(*this == a); }

size_t Matrix::rowCount() const { return rows; }

size_t Matrix::columnCount() const { return columns; }

const double* Matrix::getRawData() const { return elements.data(); }

double* Matrix::getRawData() { return elements.data(); }

std::ostream& linalg::operator<<(std::ostream& output, const Matrix& matrix) {
  for (size_t row{}; row < matrix.rowCount(); ++row) {
    size_t column{};
    for (; column < matrix.columnCount() - 1; ++column) {
      output << matrix.get(row, column) << ' ';
    }

    output << matrix.get(row, column) << '\n';
  }

  return output;
}

std::istream& linalg::operator>>(std::istream& input, Matrix& matrix) {
  size_t new_rows{}, new_columns{};
  double elem{};

  input >> new_rows >> new_columns;

  matrix.resize(new_rows, new_columns);

  for (size_t row{}; row < matrix.rowCount(); ++row) {
    for (size_t column{}; column < matrix.columnCount(); ++column) {
      input >> matrix.get(row, column);
    }
  }

  return input;
}

Matrix linalg::multiplyStrassenR(const Matrix& a, const Matrix& b,
                                 size_t baseBlockSize) {
  if (a.rowCount() <= baseBlockSize) {
    return a * b;
  }

  size_t newBlockSize = a.rowCount() / 2;

  Matrix a11(newBlockSize), a12(newBlockSize), a21(newBlockSize),
      a22(newBlockSize), b11(newBlockSize), b12(newBlockSize),
      b21(newBlockSize), b22(newBlockSize);

  for (size_t row{}; row < newBlockSize; ++row) {
    for (size_t col{}; col < newBlockSize; ++col) {
      a11.set(row, col, a.get(row, col));
      a12.set(row, col, a.get(row, col + newBlockSize));
      a21.set(row, col, a.get(row + newBlockSize, col));
      a22.set(row, col, a.get(row + newBlockSize, col + newBlockSize));

      b11.set(row, col, b.get(row, col));
      b12.set(row, col, b.get(row, col + newBlockSize));
      b21.set(row, col, b.get(row + newBlockSize, col));
      b22.set(row, col, b.get(row + newBlockSize, col + newBlockSize));
    }
  }

  Matrix p1 = multiplyStrassenR(a11 + a22, b11 + b22, baseBlockSize);
  Matrix p2 = multiplyStrassenR(a21 + a22, b11, baseBlockSize);
  Matrix p3 = multiplyStrassenR(a11, b12 - b22, baseBlockSize);
  Matrix p4 = multiplyStrassenR(a22, b21 - b11, baseBlockSize);
  Matrix p5 = multiplyStrassenR(a11 + a12, b22, baseBlockSize);
  Matrix p6 = multiplyStrassenR(a21 - a11, b11 + b12, baseBlockSize);
  Matrix p7 = multiplyStrassenR(a12 - a22, b21 + b22, baseBlockSize);

  Matrix c11{p1 + p4 - p5 + p7};
  Matrix c12{p3 + p5};
  Matrix c21{p2 + p4};
  Matrix c22{p1 - p2 + p3 + p6};

  Matrix c(a.rowCount());

  for (size_t row{}; row < newBlockSize; ++row) {
    for (size_t col{}; col < newBlockSize; ++col) {
      c.set(row, col, c11.get(row, col));
      c.set(row, col + newBlockSize, c12.get(row, col));
      c.set(row + newBlockSize, col, c21.get(row, col));
      c.set(row + newBlockSize, col + newBlockSize, c22.get(row, col));
    }
  }

  return c;
}

Matrix linalg::multiplyStrassen(const Matrix& a, const Matrix& b,
                                size_t baseBlockSize) {
  if (a.rowCount() != a.columnCount() || b.rowCount() != b.columnCount() ||
      a.columnCount() != b.rowCount()) {
    throw std::invalid_argument("Expected square matrix");
  }

  size_t matrix_size = a.rowCount();
  size_t new_size = linalg::nextPowerOfTwo(matrix_size);

  Matrix aligned_a(new_size), aligned_b(new_size);

  for (size_t row{}; row < a.rowCount(); ++row) {
    for (size_t col{}; col < a.columnCount(); ++col) {
      aligned_a.set(row, col, a.get(row, col));
      aligned_b.set(row, col, b.get(row, col));
    }
  }

  for (size_t row{a.rowCount()}; row < aligned_a.rowCount(); ++row) {
    aligned_a.set(row, row, 1);
    aligned_b.set(row, row, 1);
  }

  Matrix product{multiplyStrassenR(aligned_a, aligned_b, baseBlockSize)};

  if (matrix_size == new_size) {
    return product;
  } else {
    Matrix res(new_size);

    for (size_t row{}; row < a.rowCount(); ++row) {
      for (size_t col{}; col < a.columnCount(); ++col) {
        res.set(row, col, product.get(row, col));
      }
    }

    return res;
  }
}