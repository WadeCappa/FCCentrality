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

#include "adjacent.h"
#include "flow_cost_label.h"
#include "non_dominated_vectors.h"

#ifndef FLOWMAX_H
#define FLOWMAX_H

class FlowMaxCalculator 
{
    private: 
        std::vector<std::pair<size_t, size_t>> BuildPath(
            const std::vector<std::vector<Edge>>& local_matrix,
            const size_t& source,
            const size_t& target
        );

        std::pair<unsigned int, unsigned int> CalculatePathFlowCost(
            const std::vector<std::vector<Edge>>& local_matrix,
            const std::vector<std::pair<size_t, size_t>>& path,
            const size_t& sink
        );

        void UpdateMatrix(
            std::vector<std::vector<Edge>>& local_matrix,
            const std::vector<std::pair<size_t, size_t>>& path,
            const size_t& sink,
            const unsigned int& path_flow
        );

        unsigned int SolveFlowMax(    
            std::vector<std::vector<Edge>>& local_matrix, 
            const size_t& source, 
            const size_t& sink,
            const size_t& target
        );

        FlowCostLabel SolveFlowCostMax(
            std::vector<std::vector<Edge>>& local_matrix, 
            const size_t& source, 
            const size_t& sink,
            const size_t& target
        );

    public:
        FlowMaxCalculator(){};

        unsigned int SolveFlowBetweenness(
            std::vector<std::vector<Edge>>& local_matrix, 
            const size_t& source, 
            const size_t& sink,
            const size_t& target
        );

        FlowCostLabel SolveFlowCostBetweenness(
            std::vector<std::vector<Edge>>& local_matrix, 
            const size_t& source, 
            const size_t& sink,
            const size_t& target
        );
};

#endif