// for closeness
    // do bfs for every node v in the network
    // record the distance from v to every other node u in the network using bfs
    // add and normalize all the scores for every node, return result

#include <vector>
#include <unordered_map>
#include <queue>
#include <numeric>
#include <bits/stdc++.h>
#include <unordered_set>
#include <cmath>
#include <omp.h>
#include <mutex>

#include "adjacent.h"
#include "fc_vector.h"
#include "non_dominated_vectors.h"

#ifndef FCCLOSENESS_H
#define FCCLOSENESS_H

class CentralityCalculator // TODO: Should be called edge_list builder, should account for undirected vs directed, weighted vs unweighted
{
    private: 
        const std::vector<std::vector<Adjacent>>& adjacency_matrix;

        unsigned int GetFlowCloseness(size_t v, size_t u, unsigned int capacity, std::unordered_set<size_t> seen, std::vector<std::vector<Adjacent>>& adjacency_matrix);

    public:
        CentralityCalculator(const std::vector<std::vector<Adjacent>>& adjacency_matrix);
        std::vector<unsigned int> FlowCloseness();
        std::vector<unsigned int> FlowBetweenness();
        std::vector<std::vector<std::pair<unsigned int, unsigned int>>> FC_Closeness();
        std::vector<std::vector<std::pair<unsigned int, unsigned int>>> FC_Betweenness();
};

#endif