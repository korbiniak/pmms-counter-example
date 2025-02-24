from typing import Dict, Set, Tuple
import matplotlib.pyplot as plt
import numpy as np

import networkx as nx

def process_input(input_str: str) -> Dict[str, Dict[str, int]]:
    valuations = input_str.strip().split('\n\n')
    result: Dict[str, Dict[str, int]] = {}

    for i, valuation in enumerate(valuations):
        lines = valuation.split('\n')[1:]  # Skip the "Valuation X:" line
        result[f"Valuation {i}"] = {}

        for line in lines:
            key, value = line.split(': ')
            key_set: Set[int] = eval(key)  # Convert string representation of set to actual set
            value = int(value)

            if 0 < len(key_set) <= 2:
                result[f"Valuation {i}"][str(key_set)] = value

    return result

def create_graph(data: Dict[str, int]) -> nx.Graph:
    G = nx.Graph()
    m = max(max(eval(key)) for key in data.keys())
    G.add_nodes_from(range(1, m + 1))

    for key, value in data.items():
        key_set: Set[int] = eval(key)
        match tuple(key_set):
            case (node,):
                G.nodes[node]['weight'] = value
            case (u, v):
                G.add_edge(u, v, weight=value)
            case set:
                pass

    return G

def get_pmms_value(G: nx.Graph, a, b, c, d):
    pairs = [
        ((a, b), (c, d)),
        ((a, c), (b, d)),
        ((a, d), (b, c))
    ]
    max_value = 0

    # p stands for 'prime'.
    for (u, v), (up, vp) in pairs:
        max_value = max(max_value, min(G.edges[u, v]["weight"], G.edges[up, vp]["weight"]))

    return max_value

def create_valuation_graph(input_str: str) -> nx.Graph:
    valuations = process_input(input_str)
    G = nx.Graph()

    for val_id, val_dict in valuations.items():
        for set_str, value in val_dict.items():
            set_tuple = tuple(sorted(eval(set_str)))
            if len(set_tuple) == 2:
                G.add_node((val_id, set_tuple), value=value)
    nodes = list(G.nodes())
    for i, (val1_id, set1) in enumerate(nodes):
        val1_value = G.nodes[(val1_id, set1)]['value']
        for val2_id, set2 in nodes[i+1:]:
            # Only connect nodes from different valuations
            if val1_id != val2_id and not (set(set1) & set(set2)):
                val2_value = G.nodes[(val2_id, set2)]['value']
                # Create sets of all elements involved
                all_elements = sorted(set(set1) | set(set2))
                # Calculate PMMS values for both valuations
                pmms1 = get_pmms_value(create_graph(valuations[val1_id]),
                                     all_elements[0], all_elements[1],
                                     all_elements[2], all_elements[3])
                pmms2 = get_pmms_value(create_graph(valuations[val2_id]),
                                     all_elements[0], all_elements[1],
                                     all_elements[2], all_elements[3])
                print(f"{val1_id = } {set1 = } {pmms1 = } {val1_value = }")
                print(f"{val2_id = } {set2 = } {pmms2 = } {val2_value = }")
                # Add edge if both values are greater than their respective PMMS values
                if val1_value >= pmms1 and val2_value >= pmms2:
                    G.add_edge((val1_id, set1), (val2_id, set2))
    return G

def print_graph_edges(G: nx.Graph) -> None:
    print("Graph edges:")
    for (val1_id, set1), (val2_id, set2) in G.edges():
        print(f"Edge between {val1_id} set {set1} (value: {G.nodes[(val1_id, set1)]['value']}) and")
        print(f"           {val2_id} set {set2} (value: {G.nodes[(val2_id, set2)]['value']})")
        print()
def create_grouped_layout(G: nx.Graph) -> Dict:
    # Get all unique valuation IDs
    valuation_ids = sorted(set(node[0] for node in G.nodes()))
    num_valuations = len(valuation_ids)
    
    # Create a mapping of valuation ID to x-coordinate
    val_to_x = {val_id: i for i, val_id in enumerate(valuation_ids)}
    
    # For each valuation, count nodes and space them vertically
    val_counts = {val_id: 0 for val_id in valuation_ids}
    val_totals = {val_id: sum(1 for node in G.nodes() if node[0] == val_id) 
                 for val_id in valuation_ids}
    
    pos = {}
    for node in G.nodes():
        val_id = node[0]
        # Calculate x position based on valuation ID
        x = val_to_x[val_id]
        
        # Calculate y position to space nodes evenly in their column
        if val_totals[val_id] > 1:
            y = val_counts[val_id] - (val_totals[val_id] - 1) / 2
        else:
            y = 0
        
        pos[node] = np.array([x, y])
        val_counts[val_id] += 1
    
    return pos


    
    return pos

def plot_valuation_graph(G: nx.Graph) -> None:
    connected_nodes = [node for node in G.nodes() if G.degree(node) > 0]
    G_connected = G.subgraph(connected_nodes)
    
    if len(G_connected) == 0:
        print("No nodes with edges found - graph would be empty")
        return
    
    plt.figure(figsize=(12, 12))
    
    # Group nodes by valuation
    valuation_ids = sorted(set(node[0] for node in G_connected.nodes()))
    grouped_nodes = {val_id: [node for node in G_connected.nodes() if node[0] == val_id] 
                    for val_id in valuation_ids}
    
    # Create positions: first position valuations evenly around circle,
    # then distribute nodes within each valuation group
    pos = {}
    num_valuations = len(valuation_ids)
    for i, val_id in enumerate(valuation_ids):
        nodes = grouped_nodes[val_id]
        base_angle = (2 * np.pi * i) / num_valuations
        angle_step = 2 * np.pi / (num_valuations * max(len(nodes), 1) * 2)
        
        for j, node in enumerate(nodes):
            angle = base_angle + j * angle_step
            pos[node] = np.array([np.cos(angle), np.sin(angle)])
    
    nx.draw_networkx_edges(G_connected, pos, edge_color='gray', alpha=0.5)
    
    labels = {node: f"{node[0]}\n{node[1]}\n(val: {G_connected.nodes[node]['value']})" 
             for node in G_connected.nodes()}
    
    colors = plt.cm.Set3(np.linspace(0, 1, len(valuation_ids)))
    color_map = dict(zip(valuation_ids, colors))
    
    for val_id in valuation_ids:
        val_nodes = [node for node in G_connected.nodes() if node[0] == val_id]
        nx.draw_networkx_nodes(G_connected, pos, 
                             nodelist=val_nodes,
                             node_color=[color_map[val_id]],
                             node_size=2000,
                             alpha=0.7)
    
    nx.draw_networkx_labels(G_connected, pos, labels, font_size=8)
    
    plt.title("Valuation Graph (Connected Nodes Only)")
    plt.axis('off')
    plt.tight_layout()
    plt.show()

def main() -> None:
    input_str = ""
    while True:
        try:
            line = input()
            input_str += line + "\n"
        except EOFError:
            break
    
    G = create_valuation_graph(input_str)
    plot_valuation_graph(G)

if __name__ == "__main__":
    main()
