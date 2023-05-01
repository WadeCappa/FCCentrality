import sys, datetime, igraph, random

networks = []

def GetRandomWeight():
    return random.randint(1, 9)

def OutputEdgelist(edge_list, n):
    with open(n, "w") as f:
        for edge in edge_list:
            f.write(f"{edge[0]} {edge[1]} {edge[2]}\n")

# build graphs
for i in range(5, 11):
    number_of_vertices = 2**i
    print(number_of_vertices)
    print(f"network_v{number_of_vertices}.txt")
    g = igraph.Graph.Barabasi(number_of_vertices, directed=True)
    edge_list = g.get_edgelist()
    for i in range(len(edge_list)):
        edge_list[i] = (edge_list[i][0] + 1, edge_list[i][1] + 1, GetRandomWeight())
    OutputEdgelist(edge_list, f"network_v{number_of_vertices}.txt")
