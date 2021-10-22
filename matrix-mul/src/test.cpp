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

  if (argc != 2 && argc != 5) {
    cerr << "Please specify matrix size" << endl;
    return 1;
  }

  size_t a_rows{}, a_columns{}, b_rows{}, b_columns{};

  if (argc == 2) {
    a_rows = a_columns = b_rows = b_columns = std::stoi(argv[1]);
  } else {
    a_rows = std::stoi(argv[1]);
    a_columns = std::stoi(argv[2]);
    b_rows = std::stoi(argv[3]);
    b_columns = std::stoi(argv[4]);
  }

  Matrix a(a_rows, a_columns), b(b_rows, b_columns);

  Matrix c(a.rowCount(), b.columnCount());

  fill_matrix(a);
  fill_matrix(b);

  cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, c.rowCount(),
              c.columnCount(), a.columnCount(), 1.0, a.getRawData(),
              a.columnCount(), b.getRawData(), b.columnCount(), 0.0,
              c.getRawData(), c.columnCount());

  Matrix d = a * b;

  assert(c == d);

  return 0;
}