#include "graph.h"
#include "matrix.hpp"
#include "page_rank.h"
#include "stdafx.h"
#include "vector.hpp"

using namespace linalg;
using namespace graph;
using namespace pagerank;

int main(int argc, char** argv) {
  using std::cerr;
  using std::cout;
  using std::endl;

  if (argc != 3) {
    cerr << "Plesae specify arguments: path_to_file_with edges "
            "page_rank_output"
         << endl;
    return 1;
  }

  std::ifstream input(argv[1]);

  if (!input) {
    cerr << "Connat open file with graph: " << argv[1] << endl;
    return 1;
  }

  auto graph = DiGraph::readFromEdgeLits(input);

  input.close();

  NaivePageRank naive_pr;

  auto adj_matrix{graph.adjMatrix<double>()};

  auto naive_rank{naive_pr.compute(adj_matrix)};

  PageRank page_rank{0.85, 2'000, 1e-4};

  auto rank{page_rank.compute(adj_matrix)};

  auto node_labels{graph.getNodelabels()};

  std::ofstream out(argv[2]);

  // Node labels
  for (size_t i{}; i < node_labels.size() - 1; ++i) {
    out << node_labels[i] << ' ';
  }

  out << node_labels.at(node_labels.size() - 1) << endl;
  out << naive_rank.transpose() << rank.transpose();

  out.close();

  return 0;
}
