import sys
from igraph import *

karate = Graph.Read_Edgelist(sys.argv[1])

print(summary(karate))

# print(karate.betweenness(directed=True))
print(karate.closeness())