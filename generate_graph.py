import os

import networkx as nx

if __name__ == "__main__":
    path_to_data = os.path.join("data", "email-Eu-core.txt")

    graph = nx.read_edgelist(path_to_data, create_using=nx.DiGraph)
    graph.remove_edges_from(list(nx.selfloop_edges(graph)))

    conn_comp = list(map(lambda x: (x, len(x)), nx.algorithms.strongly_connected_components(graph)))
    conn_comp.sort(reverse=True, key=lambda x: x[1])

    subg = graph.subgraph(conn_comp[0][0])

    nx.write_edgelist(subg, os.path.join(os.path.dirname(path_to_data), "simplified.txt"), encoding="utf-8", data=False)