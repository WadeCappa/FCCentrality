#include "centrality_calculator.h"

std::vector<long double> CentralityCalculator::DEBUG_CalculateCloseness(const std::vector<std::vector<Adjacent>>& adjacency_matrix)
{
    typedef struct queueVertex {
        size_t vertex;
        unsigned int distance;
    } QueueVertex;

    std::vector<std::vector<unsigned>> closeness_scores(
        adjacency_matrix.size(), std::vector<unsigned>(adjacency_matrix.size(), 0)
    );

    for (size_t v = 0; v < adjacency_matrix.size(); v++)
    {
        std::queue<QueueVertex> vertex_queue;
        std::unordered_set<size_t> seen;
        vertex_queue.push({v, 0});
        seen.insert(v);

        while(!vertex_queue.empty())
        {
            QueueVertex next = vertex_queue.front();
            vertex_queue.pop();

            closeness_scores[v][next.vertex] = next.distance;

            for (const auto & u : adjacency_matrix[next.vertex])
            {
                if (seen.find(u.vertex) == seen.end())
                {
                    vertex_queue.push({u.vertex, next.distance + 1});
                    seen.insert(u.vertex);
                }
            }
        }
    }

    std::vector<long double> res(adjacency_matrix.size(), 0);

    for (size_t v = 0; v < closeness_scores.size(); v++)
    {
        res[v] = (long double)(adjacency_matrix.size() - 1) / (long double)std::accumulate(closeness_scores[v].begin(), closeness_scores[v].end(), 0);
    }

    return res;
}