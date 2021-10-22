#include <cassert>

#include "cblas.h"
#include "matrix.h"
#include "stdafx.h"
#include "utils.h"

int main(int argc, char** argv) {
  using std::cerr;
  using std::cout;
  using std::endl;
  using namespace linalg;

  if (argc != 2) {
    cerr << "Please specify matrix size" << endl;
    return 1;
  }

  size_t matrix_size{};

  if (argc == 2) {
    matrix_size = std::stoi(argv[1]);
  }

  Matrix a(matrix_size), b(matrix_size);

  Matrix c(a.rowCount(), b.columnCount());

  fill_matrix(a);
  fill_matrix(b);

  cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, c.rowCount(),
              c.columnCount(), a.columnCount(), 1.0, a.getRawData(),
              a.columnCount(), b.getRawData(), b.columnCount(), 0.0,
              c.getRawData(), c.columnCount());

  Matrix d = multiplyStrassen(a, b, 16);

  assert(c == d);

  return 0;
}