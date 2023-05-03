import igraph, sys, os, time, networkx

file = "..\\networks\\real_world\\lastfm_asia_edges.csv"
print(file)

ig = igraph.Graph.Read_Edgelist(file, directed=True)
start_time = time.time()
ig.eigenvector_centrality(directed=False)
print("--- %s seconds for eigenvector centrality ---" % (time.time() - start_time))

ng = networkx.read_edgelist(file, delimiter=" ", nodetype=int)

start_time = time.time()
networkx.katz_centrality_numpy(ng)
print("--- %s seconds for katz index centrality ---" % (time.time() - start_time))