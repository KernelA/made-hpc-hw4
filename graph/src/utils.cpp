#include "utils.h"

size_t linalg::nextPowerOfTwo(size_t n) {
  size_t power_of_two = static_cast<int>(ceil(log2(n)));
  return 1 << power_of_two;
}