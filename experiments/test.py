import sys
import datetime
from igraph import *

karate = Graph.Read_Edgelist(sys.argv[1])
print("loaded network...")

# print(summary(karate))

# print(karate.betweenness(directed=True))

start_time = datetime.datetime.now()
closeness = karate.closeness()
stop_time = datetime.datetime.now()

result = stop_time - start_time

print(f"{result.seconds}, {result.microseconds}")

bestscore = 0
bestvertex = 0
for i, v in enumerate(closeness):
    if v > bestscore:
        bestscore = v
        bestvertex = i 

print(f"{bestvertex} -> {bestscore}")