#include "centrality_calculator.h"

        
        
CentralityCalculator::CentralityCalculator(const std::vector<std::vector<Adjacent>>& adjacency_matrix)
    : adjacency_matrix(adjacency_matrix)
{
}

std::vector<long double> CentralityCalculator::FlowCloseness()
{
    typedef struct queueVertex {
        size_t vertex;
        unsigned int capacity;
    } QueueVertex;

    std::vector<long unsigned int> closeness_scores(this->adjacency_matrix.size(), 0);

    #pragma omp parallel for
    for (size_t v = 0; v < this->adjacency_matrix.size(); v++)
    {
        for (size_t u = v + 1; u < this->adjacency_matrix.size(); u++)
        {
            std::vector<std::vector<Adjacent>> local_matrix = this->adjacency_matrix;

            unsigned int res = this->GetFlowCloseness(v, u, INT_MAX, std::unordered_set<size_t>(), local_matrix);
            closeness_scores[v] += res;
            closeness_scores[u] += res;
            // std::queue<QueueVertex> vertex_queue;
            // std::unordered_set<size_t> seen;
            // vertex_queue.push({v, INT_MAX});
            // seen.insert(v);

            // while(!vertex_queue.empty())
            // {
            //     QueueVertex next = vertex_queue.front();
            //     vertex_queue.pop();

            //     if (next.vertex == u)
            //     {
            //         closeness_scores[v] += next.capacity;
            //         closeness_scores[u] += next.capacity;
            //         std::cout << "plus " << next.capacity << " to vertex " << v << " and " << u << std::endl;
            //     }           

            //     else 
            //     {
            //         for (const auto & k : this->adjacency_matrix[next.vertex])
            //         {
            //             vertex_queue.push({k.vertex, std::min(next.capacity, k.weight)});
            //             seen.insert(k.vertex);
            //         }
            //     }
            // }
            for (const auto & e: closeness_scores)
            {
                std::cout << e << ", ";
            }
            std::cout << "::" << std::endl;
        }
    }

    // std::vector<long double> res(adjacency_matrix.size(), 0);
    std::vector<long double> res;

    for (const auto & v : closeness_scores)
    {
        res.push_back((long double)v);
    }

    // #pragma omp parallel for
    // for (size_t v = 0; v < closeness_scores.size(); v++)
    // {
    //     long double normalized_closeness = (long double)(adjacency_matrix.size() - 1) / (long double)closeness_scores[v];
    //     res[v] = std::isinf(normalized_closeness) ? 0 : normalized_closeness;
    // }

    return res;
}


std::vector<long double> CentralityCalculator::FC_Closeness()
{
    std::vector<long double> res; 
    // distance to cost, or cost to distance. Maximize the flow (distance), minimize the cost (edge weights)
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
    // std::cout << v << ", " << u << ", " << capacity << ", " << std::endl;

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