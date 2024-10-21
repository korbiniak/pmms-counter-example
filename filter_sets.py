from typing import Dict, Set, List, Tuple, Union
import networkx as nx
import matplotlib.pyplot as plt
import numpy as np
from netgraph import InteractiveGraph
import netgraph
from matplotlib.figure import Figure
from matplotlib.axes import Axes
import matplotlib

def process_input(input_str: str) -> Dict[str, Dict[str, int]]:
    valuations = input_str.strip().split('\n\n')
    result: Dict[str, Dict[str, int]] = {}

    for i, valuation in enumerate(valuations):
        lines = valuation.split('\n')[1:]  # Skip the "Valuation X:" line
        result[f"Valuation {i}"] = {}

        for line in lines:
            # print(line)
            key, value = line.split(': ')
            key_set: Set[int] = eval(key)  # Convert string representation of set to actual set
            value = int(value)

            if 0 < len(key_set) <= 2:
                result[f"Valuation {i}"][str(key_set)] = value

    return result

def create_jittered_layout(filtered_data: Dict[str, Dict[str, int]]) -> Dict[int, Tuple[float, float]]:
    max_nodes = max(max(max(eval(key)) for key in data.keys()) for data in filtered_data.values())
    base_pos = nx.circular_layout(range(1, max_nodes + 1))
    return {node: (x + np.random.normal(0, 0.05), y + np.random.normal(0, 0.05))
            for node, (x, y) in base_pos.items()}

def create_graph(data: Dict[str, int]) -> nx.Graph:
    G = nx.Graph()
    m = max(max(eval(key)) for key in data.keys())
    G.add_nodes_from(range(1, m + 1))

    for key, value in data.items():
        key_set: Set[int] = eval(key)
        match tuple(key_set):
            case (node,):
                G.nodes[node]['value'] = value
            case (u, v):
                G.add_edge(u, v, weight=value)
            case set:
                pass

    return G

def get_edge_weights(G: nx.Graph) -> Dict[Tuple[int, int], int]:
    return {(u, v): d['weight'] for u, v, d in G.edges(data=True)}

def get_pmms_value(G: nx.Graph, a, b, c, d):
    pairs = [
        ((a, b), (c, d)),
        ((a, c), (b, d)),
        ((a, d), (b, c))
    ]
    max_value = 0

    for (u, v), (up, vp) in pairs:
        max_value = max(max_value, min(G.edges[u, v]["weight"], G.edges[up, vp]["weight"]))

    return max_value

def get_mouseover_highlight_mapping(G: nx.Graph) -> Dict[Union[int, Tuple[int, int]], List[Union[int, Tuple[int, int]]]]:
    mapping = {}
    for u, v, w in G.edges.data("weight"):
        mapping[(u, v)] = [(u, v), u, v]
        for up, vp in G.edges:
            if up == u or up == v or vp == u or vp == v:
                continue

            pmms = get_pmms_value(G, u, v, up, vp)
            print(u, v, up, vp, pmms)
            if w >= pmms:
                mapping[(u, v)] += [(up, vp), up, vp]

    return mapping

def draw_interactive_graph(G: nx.Graph, pos: Dict[int, Tuple[float, float]], ax: Axes, valuation: str):
    edge_weights = get_edge_weights(G)
    node_sizes = {node: 5 for node in G.nodes()}
    node_labels = {node: f"{node}\n({G.nodes[node].get('value', '')})" for node in G.nodes()}
    edge_labels = nx.get_edge_attributes(G, 'weight')
    mouseover_highlight_mapping = get_mouseover_highlight_mapping(G)
    print(mouseover_highlight_mapping)

    g = InteractiveGraph(
        G,
        node_layout=pos,
        node_size=node_sizes,
        node_labels=node_labels,
        node_label_offset=0.1,
        edge_labels=edge_labels,
        edge_label_offset=0.1,
        edge_width=2,
        edge_cmap='viridis',
        # WARNING: I changed the library to support this!
        mouseover_highlight_mapping=mouseover_highlight_mapping,
        ax=ax
    )

    ax.set_title(f"Graph for {valuation}")
    ax.axis('off')

    return g, ax

def draw_graphs(filtered_data: Dict[str, Dict[str, int]]) -> None:
    num_valuations = len(filtered_data)
    fig, axs = plt.subplots(1, num_valuations, figsize=(8*num_valuations, 6), squeeze=False)
    axs = axs.flatten()

    jittered_pos = create_jittered_layout(filtered_data)

    graphs = []

    for i, (valuation, data) in enumerate(filtered_data.items()):
        G = create_graph(data)
        pos = {node: jittered_pos[node] for node in G.nodes()}
        graph, ax = draw_interactive_graph(G, pos, axs[i], valuation)
        graphs.append((G, graph, ax))


    plt.tight_layout()
    plt.show()

def main() -> None:
    input_str: str = ""
    while True:
        try:
            line = input()
            input_str += line + "\n"
        except EOFError:
            break

    filtered_data: Dict[str, Dict[str, int]] = process_input(input_str)

    for valuation, data in filtered_data.items():
        print(f"{valuation}:")
        for key, value in data.items():
            print(f"{key}: {value}")
        print()  # Add a blank line between valuations

        print(data.keys())
    draw_graphs(filtered_data)

if __name__ == "__main__":
    main()
