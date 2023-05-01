#include "flow_max_calculator.h"

std::vector<std::pair<size_t, size_t>> FlowMaxCalculator::BuildPath(
    const std::vector<std::vector<Edge>>& local_matrix,
    const size_t& source,
    const size_t& target
)
{
    std::vector<std::pair<size_t, size_t>> path(local_matrix.size(), std::make_pair(-1,-1)); // pointers to edges in local_matrix

    std::unordered_set<size_t> seen;
    seen.insert(source);

    std::queue<size_t> q;
    q.push(source);

    while (!q.empty())
    {
        size_t current = q.front();
        q.pop();
    
        for (size_t i = 0; i < local_matrix[current].size(); i++)
        {
            Edge e = local_matrix[current][i];
            if (seen.find(e.target) == seen.end() && e.target != source && e.capacity > e.flow && e.target != target)
            {
                path[e.target] = std::make_pair(current, i);
                seen.insert(e.target);
                q.push(e.target);
            }
        }
    }

    return path;
}

std::pair<unsigned int, unsigned int> FlowMaxCalculator::CalculatePathFlowCost(
    const std::vector<std::vector<Edge>>& local_matrix,
    const std::vector<std::pair<size_t, size_t>>& path,
    const size_t& sink
)
{
    unsigned int path_flow = INT_MAX;
    unsigned int distance = 0;
    
    for (size_t i = sink; path[i].first != -1; i = local_matrix[path[i].first][path[i].second].parent)
    {
        Edge edge = local_matrix[path[i].first][path[i].second];
        path_flow = std::min(path_flow, edge.capacity - edge.flow);
        distance++; 
    }

    return std::make_pair(path_flow, distance);
}

void FlowMaxCalculator::UpdateMatrix(
    std::vector<std::vector<Edge>>& local_matrix,
    const std::vector<std::pair<size_t, size_t>>& path,
    const size_t& sink,
    const unsigned int& path_flow
)
{
    for (size_t i = sink; path[i].first != -1; i = local_matrix[path[i].first][path[i].second].parent)
    {                
        Edge& edge = local_matrix[path[i].first][path[i].second];
        edge.flow = edge.flow + path_flow;
    }
}


unsigned int FlowMaxCalculator::SolveFlowMax(    
    std::vector<std::vector<Edge>>& local_matrix, 
    const size_t& source, 
    const size_t& sink,
    const size_t& target
)
{
    unsigned int res;
    unsigned int flow = 0;

    while (true)
    {
        std::vector<std::pair<size_t, size_t>> path = this->BuildPath(local_matrix, source, target);

        if (path[sink].first != -1)
        {       
            std::pair<unsigned int, unsigned int> flow_cost = this->CalculatePathFlowCost(local_matrix, path, sink);
            this->UpdateMatrix(local_matrix, path, sink, flow_cost.first);

            res += flow_cost.first;
        }
        else
        {
            return res;
        }
    }
}

FlowCostLabel FlowMaxCalculator::SolveFlowCostMax(
    std::vector<std::vector<Edge>>& local_matrix, 
    const size_t& source, 
    const size_t& sink,
    const size_t& target
)
{
    FlowCostLabel res;
    unsigned int flow = 0;

    while (true)
    {
        std::vector<std::pair<size_t, size_t>> path = this->BuildPath(local_matrix, source, target);

        if (path[sink].first != -1)
        {       
            std::pair<unsigned int, unsigned int> flow_cost = this->CalculatePathFlowCost(local_matrix, path, sink);
            this->UpdateMatrix(local_matrix, path, sink, flow_cost.first);

            FlowCostLabel temp(flow_cost.first, flow_cost.second);
            res = res + temp;
        }
        else
        {
            return res;
        }
    }
}

unsigned int FlowMaxCalculator::SolveFlowBetweenness(    
    std::vector<std::vector<Edge>>& local_matrix, 
    const size_t& source, 
    const size_t& sink,
    const size_t& target
)
{
    this->SolveFlowMax(local_matrix, source, sink, target);
    return this->SolveFlowMax(local_matrix, source, sink, -1);
}

FlowCostLabel FlowMaxCalculator::SolveFlowCostBetweenness(
    std::vector<std::vector<Edge>>& local_matrix, 
    const size_t& source, 
    const size_t& sink,
    const size_t& target
)
{
    this->SolveFlowCostMax(local_matrix, source, sink, target);
    return this->SolveFlowCostMax(local_matrix, source, sink, -1);
}