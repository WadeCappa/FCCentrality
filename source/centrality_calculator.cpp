#include "centrality_calculator.h"

        
        
CentralityCalculator::CentralityCalculator(const std::vector<std::vector<Adjacent>>& adjacency_matrix)
    : adjacency_matrix(adjacency_matrix)
{
}

std::vector<unsigned int> CentralityCalculator::FlowCloseness()
{
    typedef struct queueVertex {
        size_t vertex;
        unsigned int capacity;
    } QueueVertex;

    std::vector<unsigned int> closeness_scores(this->adjacency_matrix.size(), 0);

    #pragma omp parallel for
    for (size_t v = 0; v < this->adjacency_matrix.size(); v++)
    {
        for (size_t u = v + 1; u < this->adjacency_matrix.size(); u++)
        {
            std::vector<std::vector<Adjacent>> local_matrix = this->adjacency_matrix;

            unsigned int res = this->GetFlowCloseness(v, u, INT_MAX, std::unordered_set<size_t>(), local_matrix);
            closeness_scores[v] += res;
            closeness_scores[u] += res;
        }
    }

    return closeness_scores;
}


std::vector<unsigned int> CentralityCalculator::FC_Closeness()
{
    std::vector<unsigned int> res; 
    std::unordered_map<int, int> nondominating_vector;

    return res;
}


unsigned int CentralityCalculator::GetFlowCloseness(
    size_t v, // current vertex
    size_t u, // end point
    unsigned int capacity, 
    std::unordered_set<size_t> seen,
    std::vector<std::vector<Adjacent>>& local_matrix
)
{
    if (v == u || capacity <= 0)
        return capacity;

    seen.insert(v);

    unsigned int res = 0;
    for (auto & k : local_matrix[v])
    {
        if (seen.find(k.vertex) == seen.end())
        {
            unsigned int new_capacity = std::min(capacity, k.weight);
            k.weight -= new_capacity;
            res += this->GetFlowCloseness(k.vertex, u, new_capacity, seen, local_matrix);
        }
    }

    return res;
}