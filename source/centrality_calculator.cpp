#include "centrality_calculator.h"

typedef struct queueVertex {
    size_t vertex;
    unsigned int capacity;
    unsigned int distance;
    std::unordered_set<size_t> seen;
    bool target;
} QueueVertex;
        
CentralityCalculator::CentralityCalculator(const std::vector<std::vector<Adjacent>>& adjacency_matrix)
    : adjacency_matrix(adjacency_matrix)
{
}

std::vector<std::vector<std::pair<unsigned int, unsigned int>>> CentralityCalculator::FC_Closeness()
{
    std::vector<FCVector> closeness_scores(
        this->adjacency_matrix.size(), FCVector(0, 0) // map flow -> cost
    );

    // TODO: race conditions are possible here!! Fix
    #pragma omp parallel for
    for (size_t v = 0; v < this->adjacency_matrix.size(); v++)
    {
        for (size_t u = v + 1; u < this->adjacency_matrix.size(); u++)
        {
            // run bfs
            std::vector<std::vector<Adjacent>> local_matrix = this->adjacency_matrix;

            std::queue<QueueVertex> q;
            q.push({v, INT_MAX, 0, std::unordered_set<size_t>()}); // start vertex;

            while (!q.empty())
            {
                QueueVertex k = q.front();
                q.pop();

                if (k.vertex == u)
                {
                    FCVector temp(k.capacity, k.distance);
                    closeness_scores[v].Combine(temp);
                    closeness_scores[u].Combine(temp);
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
                            q.push({p.vertex, new_capacity, k.distance + 1, k.seen});
                        }
                    }
                }
            }
        }
    }

    std::vector<std::vector<std::pair<unsigned int, unsigned int>>> res; 

    for (const auto & e : closeness_scores)
    {
        res.push_back(e.BuildNDVector());
    }

    return res;
}

std::vector<std::vector<std::pair<unsigned int, unsigned int>>> CentralityCalculator::FC_Betweenness()
{
    std::vector<FCVector> closeness_scores(
        this->adjacency_matrix.size(), FCVector(0, 0) // map flow -> cost
    );

    // TODO: race conditions are possible here!! Fix
    #pragma omp parallel for
    for (size_t target = 0; target < this->adjacency_matrix.size(); target++)
    {
        for (size_t source = 0; source < this->adjacency_matrix.size(); source++)
        {
            if (source == target)
                continue;

            for (size_t sink = source + 1; sink < this->adjacency_matrix.size(); sink++)
            {
                if (sink == target)
                    continue;

                // run bfs, traversals that end up at taget are held for later
                // after bfs, do bfs again with only target nodes

                std::vector<std::vector<Adjacent>> local_matrix = this->adjacency_matrix;
                std::queue<QueueVertex> q;
                std::queue<QueueVertex> target_q;
                q.push({source, INT_MAX, 0, std::unordered_set<size_t>(), false}); // start vertex;

                while (!q.empty())
                {
                    QueueVertex k = q.front();
                    q.pop();

                    if (k.vertex == target && k.target == false)
                    {
                        k.target = true;
                        target_q.push(k);
                    }
                    else
                    {
                        if (k.vertex == sink && k.target == true)
                        {
                            FCVector temp(k.capacity, k.distance);
                            closeness_scores[target].Combine(temp);
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
                                    q.push({p.vertex, new_capacity, k.distance + 1, k.seen, k.target});
                                }
                            }
                        }
                    }

                    if (q.empty())
                    {
                        q = target_q;
                        target_q = std::queue<QueueVertex>();
                    }
                }
            }
        }
    }

    std::vector<std::vector<std::pair<unsigned int, unsigned int>>> res; 

    for (const auto & e : closeness_scores)
    {
        res.push_back(e.BuildNDVector());
    }

    return res;
}

std::vector<unsigned int> CentralityCalculator::FlowBetweenness()
{
    std::vector<unsigned int> closeness_scores(this->adjacency_matrix.size(), 0);

    // TODO: race conditions are possible here!! Fix
    #pragma omp parallel for
    for (size_t target = 0; target < this->adjacency_matrix.size(); target++)
    {
        for (size_t source = 0; source < this->adjacency_matrix.size(); source++)
        {
            if (source == target)
                continue;
            
            for (size_t sink = source + 1; sink < this->adjacency_matrix.size(); sink++)
            {
                if (sink == target)
                    continue;

                // run bfs, traversals that end up at taget are held for later
                // after bfs, do bfs again with only target nodes

                std::vector<std::vector<Adjacent>> local_matrix = this->adjacency_matrix;
                std::queue<QueueVertex> q;
                std::queue<QueueVertex> target_q;
                q.push({source, INT_MAX, 0, std::unordered_set<size_t>(), false}); // start vertex;

                while (!q.empty())
                {
                    QueueVertex k = q.front();
                    q.pop();

                    if (k.vertex == target && k.target == false)
                    {
                        k.target = true;
                        target_q.push(k);
                    }
                    else
                    {
                        if (k.vertex == sink && k.target == true)
                        {
                            closeness_scores[target] += k.capacity;
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
                                    q.push({p.vertex, new_capacity, k.distance + 1, k.seen, k.target});
                                }
                            }
                        }
                    }

                    if (q.empty())
                    {
                        q = target_q;
                        target_q = std::queue<QueueVertex>();
                    }
                }
            }
        }
    }

    return closeness_scores;
}

std::vector<unsigned int> CentralityCalculator::FlowCloseness()
{
    std::vector<unsigned int> closeness_scores(this->adjacency_matrix.size(), 0);

    // TODO: race conditions are possible here!! Fix
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
