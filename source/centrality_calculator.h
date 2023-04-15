// for closeness
    // do bfs for every node v in the network
    // record the distance from v to every other node u in the network using bfs
    // add and normalize all the scores for every node, return result

#include <vector>
#include <queue>
#include <numeric>
#include <bits/stdc++.h>
#include <unordered_set>
#include "adjacent.h"

#ifndef FCCLOSENESS_H
#define FCCLOSENESS_H

class CentralityCalculator // TODO: Should be called edge_list builder, should account for undirected vs directed, weighted vs unweighted
{
    public:
        static std::vector<long double> DEBUG_CalculateCloseness(const std::vector<std::vector<Adjacent>>& adjacency_matrix);
};

#endif