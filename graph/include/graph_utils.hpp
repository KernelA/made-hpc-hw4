#pragma once

#include "matrix.hpp"

namespace graph {

template <typename ValueType>
linalg::Matrix<ValueType> randomAdjMatrix(size_t number_of_nodes,
                                          std::default_random_engine& engine) {
  linalg::Matrix<ValueType> adj_matrix(number_of_nodes);

  adj_matrix.randomFill(0, 1, engine);

  for (size_t row{}; row < adj_matrix.rowCount(); ++row) {
    adj_matrix.at(row, row) = static_cast<ValueType>(0);
  }

  return adj_matrix;
}

}  // namespace graph
