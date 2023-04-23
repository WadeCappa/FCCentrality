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
        FCVector();
        FCVector(unsigned int capacity, unsigned int cost);
        std::map<unsigned int, unsigned int> GetFlowCost() const;
        void DEBUG_Display() const;
        void Combine(const FCVector& vector);
        std::vector<std::pair<unsigned int, unsigned int>> BuildNDVector() const;

        FCVector& operator+(const FCVector& v);
};

#endif