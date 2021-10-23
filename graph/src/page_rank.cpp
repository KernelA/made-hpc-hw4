#include "page_rank.h"

linalg::Vector<double> pagerank::NaivePageRank::compute(
    const linalg::Matrix<double>& adj_matrix) const {
  using linalg::Vector;

  auto outComeAdjmatrix{adj_matrix.transpose()};

  Vector<double> naive_rank(outComeAdjmatrix.rowCount());

  for (size_t row{}; row < outComeAdjmatrix.rowCount(); ++row) {
    for (size_t col{}; col < outComeAdjmatrix.columnCount(); ++col) {
      naive_rank.at(row) += outComeAdjmatrix.at(row, col);
    }
  }

  naive_rank.normalizeL2();

  return naive_rank;
}

pagerank::PageRank::PageRank(double damping_factor, size_t num_iters,
                             double eps)
    : damping_factor(damping_factor), num_iters(num_iters), eps(eps) {}

linalg::Vector<double> pagerank::PageRank::compute(
    const linalg::Matrix<double>& adj_matrix) const {
  using MatrixType = linalg::Matrix<double>;
  using VectorType = linalg::Vector<double>;

  auto outComeAdjmatrix{adj_matrix.transpose()};

  for (size_t col{}; col < outComeAdjmatrix.columnCount(); ++col) {
    double sum{};

    for (size_t row{}; row < outComeAdjmatrix.rowCount(); ++row) {
      sum += outComeAdjmatrix.at(row, col);
    }

    for (size_t row{}; row < outComeAdjmatrix.rowCount(); ++row) {
      if (sum != 0) {
        outComeAdjmatrix.at(row, col) /= sum;
      }
    }
  }

  MatrixType all_ones{outComeAdjmatrix.rowCount()};
  all_ones.fill(1.0);

  MatrixType power_matrix{this->damping_factor * outComeAdjmatrix +
                          (1 - damping_factor) / outComeAdjmatrix.rowCount() *
                              all_ones};

  VectorType rank{outComeAdjmatrix.rowCount()};

  std::default_random_engine engine;
  rank.randomFill(0.0, 1.0, engine);
  rank.normalizeL2();

  VectorType prev_rank{rank};

  for (size_t i{}; i < num_iters; ++i) {
    rank = power_matrix * prev_rank;

    VectorType error{rank - prev_rank};

    rank.normalizeL2();

    if (error.squaredNormL2() < eps * eps) {
      break;
    }

    prev_rank = rank;
  }

  return rank;
}
