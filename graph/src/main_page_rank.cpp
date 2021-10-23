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

  std::ifstream input("/home/alex/made/module3/made-hpc-hw4/data/ca-GrQc.txt");

  if (!input) {
    cerr << "Connat open file with graph" << endl;
    return 1;
  }
  auto graph = DiGraph::readFrom(input);

  NaivePageRank naive_pr;

  auto naive_rank{naive_pr.compute(graph.adjMatrix<double>())};

  return 0;
}
