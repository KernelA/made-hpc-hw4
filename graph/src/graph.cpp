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

  auto from_iterator{node_list.find(from_node_num)};

  if (from_iterator == node_list.end()) {
    node_list.emplace(from_node_num,
                      NodeList::value_type::second_type{to_node_num});
  } else {
    auto& node_list = *from_iterator;
    node_list.second.emplace(to_node_num);
  }
}

graph::DiGraph graph::DiGraph::readFrom(std::ifstream& in_stream) {
  using std::getline;
  using std::string;
  using std::stringstream;

  DiGraph graph;
  stringstream line_stream;
  string from_node_name, to_node_name, line;

  while (in_stream) {
    getline(in_stream, line);

    if (in_stream.eof()) {
      break;
    }

    if (string::npos != line.find_first_of(u8'#')) {
      continue;
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
