// for closeness
    // do bfs for every node v in the network
    // record the distance from v to every other node u in the network using bfs
    // add and normalize all the scores for every node, return result

#include <vector>
#include <unordered_map>
#include <queue>
#include <stack>
#include <numeric>
#include <bits/stdc++.h>
#include <unordered_set>
#include <cmath>
#include <omp.h>
#include <mutex>

#include "edge_structs.h"
#include "flow_cost_label.h"
#include "non_dominated_vectors.h"
#include "flow_max_calculator.h"

#ifndef FCCLOSENESS_H
#define FCCLOSENESS_H

class CentralityCalculator // TODO: Should be called edge_list builder, should account for undirected vs directed, weighted vs unweighted
{
    private: 
        const std::vector<std::vector<Edge>>& adjacency_matrix;

        unsigned int GetFlowCloseness(size_t v, size_t u, unsigned int capacity, std::unordered_set<size_t> seen, std::vector<std::vector<Edge>>& adjacency_matrix);
        FlowCostLabel SolveMaxFlow(std::vector<std::vector<Edge>>& local_matrix, size_t source, size_t sink, size_t target);
        std::vector<std::vector<DualCost>> SolveForDistance(const size_t source, const size_t sink);

    public:
        CentralityCalculator(const std::vector<std::vector<Edge>>& adjacency_matrix);
        std::vector<unsigned int> FlowCloseness();
        std::vector<unsigned int> FlowBetweenness();
        std::vector<std::vector<std::pair<unsigned int, unsigned int>>> FlowCostCloseness();
        std::vector<std::vector<std::pair<unsigned int, unsigned int>>> FlowCostBetweenness();
};

#endif