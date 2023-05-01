#include <vector>
#include <unordered_map>
#include <map>
#include <queue>
#include <numeric>
#include <bits/stdc++.h>
#include <unordered_set>
#include <cmath>
#include <algorithm>

#ifndef FCVECTOR_H
#define FCVECTOR_H

class FlowCostLabel  
{
    private:
        std::map<unsigned int, unsigned int> flow_cost;

    public:
        FlowCostLabel();
        FlowCostLabel(unsigned int capacity, unsigned int cost);
        std::map<unsigned int, unsigned int> GetFlowCost() const;
        void DEBUG_Display() const;
        void Combine(const FlowCostLabel& vector);
        void Extend(const unsigned int flow, const unsigned int distance);
        void Merge(const FlowCostLabel& v);
        std::vector<std::pair<unsigned int, unsigned int>> BuildNDVector() const;

        FlowCostLabel& operator+(const FlowCostLabel& v);
        bool operator==(const FlowCostLabel& v);
        FlowCostLabel& operator+(const std::pair<unsigned int, unsigned int>& fc);
};

#endif