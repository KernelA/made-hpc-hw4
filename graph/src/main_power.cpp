#include "graph_utils.hpp"
#include "matrix.hpp"
#include "stdafx.h"

using namespace linalg;
using namespace graph;

int main(int argc, char** argv) {
  using std::cerr;
  using std::cout;
  using std::default_random_engine;
  using std::endl;

  using ValueType = ulong;

  default_random_engine engine;

  const size_t NUMBER_NODES{10};
  const size_t MAX_LENGTH{5};

  Matrix<ValueType> adj_matrix{
      randomAdjMatrix<ValueType>(NUMBER_NODES, engine)};

  for (size_t path_length{1}; path_length <= MAX_LENGTH; ++path_length) {
    auto power_matrix{adj_matrix.powerOf(path_length)};

    size_t start_node{};

    for (size_t end_node{}; end_node < power_matrix.columnCount(); ++end_node) {
      cout << "Number of paths with length " << path_length
           << " between: " << start_node << " and " << end_node << ' '
           << power_matrix.at(start_node, end_node) << endl;
    }
  }

  return 0;
}
