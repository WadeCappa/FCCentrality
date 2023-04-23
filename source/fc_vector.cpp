#include "fc_vector.h"

FCVector::FCVector()
{
    this->flow_cost[0] = 0;
}

FCVector::FCVector(unsigned int capacity, unsigned int cost)
{
    for (unsigned int i = 0; i <= capacity; i++)
    {
        this->flow_cost[i] = i * cost;
    }
}

void FCVector::DEBUG_Display() const
{
    auto vtr = this->BuildNDVector();
    for (const auto & e : vtr)
    {
        std::cout << "(" << e.first << "," << e.second << ")";
    }
    std::cout << std::endl;
}

std::map<unsigned int, unsigned int> FCVector::GetFlowCost() const
{
    return this->flow_cost;
}

std::vector<std::pair<unsigned int, unsigned int>> FCVector::BuildNDVector() const
{
    std::vector<std::pair<unsigned int, unsigned int>> nd_res(this->flow_cost.begin(), this->flow_cost.end());
    std::reverse(nd_res.begin(), nd_res.end());
    return nd_res;
}


FCVector& FCVector::operator+(const FCVector& v)
{
    std::map<unsigned int, unsigned int> new_cost_flow;

    for (const auto & p : v.GetFlowCost())
    {
        for (const auto & q : this->flow_cost)
        {
            unsigned int new_flow = p.first + q.first;
            if (new_cost_flow.find(new_flow) == new_cost_flow.end())
                new_cost_flow[new_flow] = p.second + q.second;
            else 
                new_cost_flow[new_flow] = std::min(p.second + q.second, new_cost_flow[new_flow]);
        }
    }

    this->flow_cost = new_cost_flow;

    return *this;
}