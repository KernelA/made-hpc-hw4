#pragma once

#include "matrix.hpp"
#include "stdafx.h"
#include "vector.hpp"

namespace pagerank {

class BasePageRank {
 public:
  BasePageRank() = default;

  BasePageRank(const BasePageRank& page_rank) = default;

  virtual ~BasePageRank() = default;

  virtual linalg::Vector<double> compute(
      const linalg::Matrix<double>& adj_matrix) const = 0;

 private:
};

class NaivePageRank : public BasePageRank {
 public:
  linalg::Vector<double> compute(
      const linalg::Matrix<double>& adj_matrix) const override;
};

class PageRank : public BasePageRank {
 public:
  explicit PageRank(double damping_factor, size_t num_iters, double eps);

  linalg::Vector<double> compute(
      const linalg::Matrix<double>& adj_matrix) const override;

 private:
  double damping_factor, eps;
  size_t num_iters;
};

}  // namespace pagerank
