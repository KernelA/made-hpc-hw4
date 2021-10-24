#include "matrix.hpp"
#include "stdafx.h"
#include "utils.h"

int main(int argc, char** argv) {
  using std::cerr;
  using std::cout;
  using std::endl;
  using namespace linalg;
  using MatrixType = Matrix<long>;

  if (argc != 2) {
    cerr << "Please specify matrix size" << endl;
    return 1;
  }

  size_t matrix_size{};

  matrix_size = std::stoi(argv[1]);

  std::default_random_engine engine;

  MatrixType a(matrix_size), b(matrix_size);
  MatrixType c(a.rowCount(), b.columnCount());

  a.randomFill(-100, 100, engine);
  b.randomFill(-100, 100, engine);

  Matrix d = multiplyStrassen(a, b, 16);

  assert(a * b == d);

  return 0;
}
