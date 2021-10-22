#include "utils.h"

void linalg::fill_matrix(Matrix& a, double min_value, double max_value) {
  using std::default_random_engine;
  using std::uniform_real_distribution;

  default_random_engine engine;

  uniform_real_distribution<double> random_engine(min_value, max_value);

  for (size_t row{}; row < a.rowCount(); ++row) {
    for (size_t col{}; col < a.columnCount(); ++col) {
      a.set(row, col, random_engine(engine));
    }
  }
}

size_t linalg::nextPowerOfTwo(size_t n) {
  size_t power_of_two = static_cast<int>(ceil(log2(n)));
  return 1 << power_of_two;
}