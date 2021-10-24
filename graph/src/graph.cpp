#include "graph.h"

void graph::DiGraph::add_edge(const std::string& from_node_name,
                              const std::string& to_node_name) {
  size_t from_node_num{};
  {
    auto from_node_num_it{node_mapping.find(from_node_name)};

    if (from_node_num_it == node_mapping.end()) {
      from_node_num = node_mapping.size();
      node_mapping.emplace(from_node_name, from_node_num);
    } else {
      from_node_num = (*from_node_num_it).second;
    }
  }

  size_t to_node_num{};

  {
    auto to_node_num_it{node_mapping.find(to_node_name)};

    if (to_node_num_it == node_mapping.end()) {
      to_node_num = node_mapping.size();
      node_mapping.emplace(to_node_name, to_node_num);
    } else {
      to_node_num = (*to_node_num_it).second;
    }
  }

  auto from_iterator{node_lists.find(from_node_num)};

  if (from_iterator == node_lists.end()) {
    node_lists.emplace(from_node_num,
                       NodeList::value_type::second_type{to_node_num});
  } else {
    auto& node_list = *from_iterator;
    node_list.second.emplace(to_node_num);
  }
}

graph::DiGraph graph::DiGraph::readFromEdgeLits(std::ifstream& in_stream,
                                                const char comment) {
  using std::getline;
  using std::string;
  using std::stringstream;

  DiGraph graph;
  stringstream line_stream;
  string from_node_name, to_node_name, line;

  while (!in_stream.eof()) {
    getline(in_stream, line);

    if (in_stream.eof()) {
      break;
    }

    if (in_stream.fail()) {
      throw std::runtime_error("IO error");
    }

    bool is_start_with_comment{false};

    for (const auto& symbol : line) {
      if (isspace(symbol)) {
        continue;
      }

      if (comment == symbol) {
        is_start_with_comment = true;
      } else {
        is_start_with_comment = false;
      }

      break;
    }

    stringstream line_stream(line);

    line_stream >> from_node_name >> to_node_name;

    graph.add_edge(from_node_name, to_node_name);
  }

  graph.buildInversemapping();

  return graph;
}

void graph::DiGraph::buildInversemapping() {
  num2node.clear();
  for (const auto& nodeNumPair : node_mapping) {
    num2node.emplace(nodeNumPair.second, nodeNumPair.first);
  }
}

std::vector<graph::DiGraph::NodeLabel> graph::DiGraph::getNodelabels() const {
  std::vector<NodeLabel> labels(num2node.size(), NodeLabel());

  for (const auto& nodeIndexLabel : num2node) {
    labels.at(nodeIndexLabel.first) = nodeIndexLabel.second;
  }

  return labels;
}
