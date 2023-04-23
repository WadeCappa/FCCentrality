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

class FCVector 
{
    private:
        std::map<unsigned int, unsigned int> flow_cost;

    public:
        FCVector(unsigned int capacity, unsigned int cost);
        void DEBUG_Display();
        void Combine(const std::map<unsigned int, unsigned int>& vector);
        std::vector<std::pair<unsigned int, unsigned int>> BuildNDVector();

        std::map<unsigned int, unsigned int> GetFlowCost();
};

#endif