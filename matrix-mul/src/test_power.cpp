#include "matrix.hpp"
#include "stdafx.h"
#include "utils.h"

int main(int argc, char** argv) {
  using std::cerr;
  using std::cout;
  using std::endl;
  using namespace linalg;

  using ValueType = long;

  if (argc != 3) {
    cerr << "Please specify matrix size and power" << endl;
    return 1;
  }

  size_t matrix_size{};

  matrix_size = std::stoi(argv[1]);

  uint power = std::stoi(argv[2]);

  Matrix<ValueType> a(matrix_size);

  std::default_random_engine engine;

  a.randomFill(-50, 50, engine);

  Matrix<ValueType> power_a{a.powerOf(power)};

  Matrix true_power{a};

  for (size_t i{1}; i < power; ++i) {
    true_power *= a;
  }

  assert(power_a == true_power);

  return 0;
}