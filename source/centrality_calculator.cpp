#include "centrality_calculator.h"

        
        
CentralityCalculator::CentralityCalculator(const std::vector<std::vector<Adjacent>>& adjacency_matrix)
    : adjacency_matrix(adjacency_matrix)
{
}

std::vector<unsigned int> CentralityCalculator::FlowCloseness()
{
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


std::vector<std::vector<std::pair<unsigned int, unsigned int>>> CentralityCalculator::FC_Closeness()
{
    typedef struct queueVertex {
        size_t vertex;
        unsigned int capacity;
        unsigned int distance;
        std::unordered_set<size_t> seen;
    } QueueVertex;

    std::vector<std::unordered_map<unsigned int, unsigned int>> closeness_scores(
        this->adjacency_matrix.size(), std::unordered_map<unsigned int, unsigned int>() // map flow -> cost
    );

    #pragma omp parallel for
    for (size_t v = 0; v < this->adjacency_matrix.size(); v++)
    {
        for (size_t u = v + 1; u < this->adjacency_matrix.size(); u++)
        {
            // run bfs
            std::vector<std::vector<Adjacent>> local_matrix = this->adjacency_matrix;
            std::vector<FCVector*> path_scores;

            std::queue<QueueVertex> q;
            QueueVertex start_element;
            // start_element.vertex = v;
            // start_element.capacity = INT_MAX;
            // start_element.distance = 0;

            start_element = {v, INT_MAX, 0, std::unordered_set<size_t>()};
            q.push(start_element); // start vertex;

            while (!q.empty())
            {
                QueueVertex k = q.front();
                q.pop();

                if (k.vertex == u)
                {
                    path_scores.push_back(new FCVector(k.capacity, k.distance));
                }
                else if (k.capacity > 0)
                {
                    k.seen.insert(k.vertex);
                    for (auto & p : local_matrix[k.vertex])
                    {
                        if (k.seen.find(p.vertex) == k.seen.end())
                        {
                            unsigned int new_capacity = std::min(k.capacity, p.weight);
                            p.weight -= new_capacity;
                            QueueVertex next_element;
                            // next_element.vertex = p.vertex;
                            // next_element.capacity = new_capacity;
                            // next_element.distance = k.distance + 1; 
                            // next_element.seen = k.seen;
                            next_element = {p.vertex, new_capacity, k.distance + 1, k.seen};
                            q.push(next_element);
                        }
                    }
                }
            }

            std::cout << "between " << v << " and " << u << std::endl;
            FCVector* vector_res = new FCVector(0,0);
            for (auto & p : path_scores)
            {
                vector_res->Combine(p->GetFlowCost());
                delete p;
            }

            vector_res->DEBUG_Display();
        }
    }

    std::vector<std::vector<std::pair<unsigned int, unsigned int>>> res; 

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

FCVector* CentralityCalculator::FCRecusive(
    size_t v, // current vertex
    size_t u, // end point
    unsigned int capacity, 
    unsigned int distance,
    std::unordered_set<size_t> seen,
    std::vector<std::vector<Adjacent>>& local_matrix
)
{
    if (v == u || capacity <= 0)
        return new FCVector(capacity, distance);

    seen.insert(v);

    FCVector* res = new FCVector(0, 0);
    for (auto & k : local_matrix[v])
    {
        if (seen.find(k.vertex) == seen.end())
        {
            unsigned int new_capacity = std::min(capacity, k.weight);
            k.weight -= new_capacity;
            FCVector* local_res = this->FCRecusive(k.vertex, u, new_capacity, distance + 1, seen, local_matrix);

        }
    }

    return res;
}