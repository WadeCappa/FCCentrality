import igraph, sys, os, time, networkx

for network in os.listdir(sys.argv[1]):
    file = '/'.join([sys.argv[1], network])
    print(file)

    ig = igraph.Graph.Read_Edgelist(file, directed=False)
    start_time = time.time()
    ig.eigenvector_centrality(directed=False)
    print("--- %s seconds for eigenvector centrality ---" % (time.time() - start_time))

    ng = networkx.read_edgelist(file, delimiter=" ", nodetype=int, data=(("weight", int),))

    start_time = time.time()
    networkx.katz_centrality(ng)
    print("--- %s seconds for katz index centrality ---" % (time.time() - start_time))
