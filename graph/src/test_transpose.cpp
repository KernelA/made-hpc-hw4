
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

  MatrixType a(matrix_size);

  a.randomFill(-100, 100, engine);

  auto transposed = a.transpose();

  for (size_t row{}; row < a.rowCount(); ++row) {
    for (size_t col{}; col < a.columnCount(); ++col) {
      assert(a.at(row, col) == transposed.at(col, row));
    }
  }

  return 0;
}
