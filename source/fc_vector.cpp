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

void FCVector::Extend(const unsigned int flow, const unsigned int distance)
{
    std::map<unsigned int, unsigned int> new_flow_cost;

    for (auto & p : this->flow_cost)
    {
        new_flow_cost[p.first + flow] = this->flow_cost[p.first] + (flow * distance);
    }

    this->flow_cost = new_flow_cost;
}


void FCVector::Merge(const FCVector& v)
{
    for (const auto & l : v.GetFlowCost())
    {
        unsigned int old_value = this->flow_cost.find(l.first) != this->flow_cost.end() ? this->flow_cost[l.first] : INT_MAX ;
        this->flow_cost[l.first] = std::min(old_value, l.second);
    }
}

void FCVector::Combine(const FCVector& v)
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
}

FCVector& FCVector::operator+(const FCVector& v)
{
    this->Combine(v);

    return *this;
}

// FCVector& FCVector::operator+(const std::pair<unsigned int, unsigned int>& fc)
// {
//     for (const auto & p : this->flow_cost)
//     {
    
//     }

//     return *this;
// }

bool FCVector::operator==(const FCVector& v)
{
    return v.GetFlowCost() == this->GetFlowCost();
}


FCVector FCVector::Copy()
{
    return *this;
}