#pragma once

#include "matrix.hpp"
#include "stdafx.h"

namespace graph {

// Directed graph
class DiGraph {
  using NodeNum = size_t;
  using NodeLabel = std::string;
  using NodeList = std::map<NodeNum, std::set<NodeNum>>;
  using NodeMapping = std::map<NodeLabel, NodeNum>;
  using IndexNodeMapping = std::map<NodeNum, NodeLabel>;

 public:
  DiGraph() = default;

  DiGraph(const DiGraph&) = default;

  void add_edge(const std::string& from, const std::string& to);

  template <typename AdjValue>
  linalg::Matrix<AdjValue> adjMatrix() const {
    linalg::Matrix<AdjValue> adj_matrix(node_mapping.size());

    for (const auto& kv : node_list) {
      const auto& node_list{kv.second};

      for (const auto& to_node_num : node_list) {
        adj_matrix.at(kv.first, to_node_num) = 1;
      }
    }

    return adj_matrix;
  }

  static DiGraph readFrom(std::ifstream& in_stream);

  ~DiGraph() = default;

 private:
  void buildInversemapping();

  NodeList node_list;
  NodeMapping node_mapping;
  IndexNodeMapping num2node;
};
}  // namespace graph
