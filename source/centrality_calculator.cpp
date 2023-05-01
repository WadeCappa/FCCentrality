#include "centrality_calculator.h"

typedef struct queueVertex {
    size_t vertex;
    unsigned int capacity;
    unsigned int distance;
    std::unordered_set<size_t> seen;
    bool target = false;
} QueueVertex;
        
CentralityCalculator::CentralityCalculator(const std::vector<std::vector<Edge>>& adjacency_matrix)
    : adjacency_matrix(adjacency_matrix)
{
}

std::vector<std::vector<std::pair<unsigned int, unsigned int>>> CentralityCalculator::FC_Closeness()
{
    NonDominatedVectors<FCVector> closeness_scores(this->adjacency_matrix.size());

    #pragma omp parallel for
    for (size_t v = 0; v < this->adjacency_matrix.size(); v++)
    {
        for (size_t u = v + 1; u < this->adjacency_matrix.size(); u++)
        {
            // run bfs
            std::vector<std::vector<Edge>> local_matrix = this->adjacency_matrix;

            std::queue<QueueVertex> q;
            q.push({v, INT_MAX, 0, std::unordered_set<size_t>()}); // start vertex;

            while (!q.empty())
            {
                QueueVertex k = q.front();
                q.pop();

                if (k.vertex == u)
                {
                    FCVector temp(k.capacity, k.distance);

                    closeness_scores.Update(v, temp);
                    closeness_scores.Update(u, temp);
                }
                else if (k.capacity > 0)
                {
                    k.seen.insert(k.vertex);
                    for (auto & p : local_matrix[k.vertex])
                    {
                        if (k.seen.find(p.target) == k.seen.end())
                        {
                            unsigned int new_capacity = std::min(k.capacity, p.capacity);
                            p.capacity -= new_capacity;
                            q.push({p.target, new_capacity, k.distance + 1, k.seen});
                        }
                    }
                }
            }
        }
    }

    std::vector<std::vector<std::pair<unsigned int, unsigned int>>> res; 

    for (const auto & e : closeness_scores.Convert())
    {
        res.push_back(e.BuildNDVector());
    }

    return res;
}

std::vector<std::vector<std::pair<unsigned int, unsigned int>>> CentralityCalculator::FC_Betweenness()
{
    NonDominatedVectors<FCVector> betweenness_scores(this->adjacency_matrix.size());

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

                auto copy = this->adjacency_matrix;
                FCVector raw_score = this->SolveMaxFlow(copy, source, sink, target);
                betweenness_scores.Update(target, raw_score);
            }
        }
    }

    std::vector<std::vector<std::pair<unsigned int, unsigned int>>> res; 

    for (const auto & e : betweenness_scores.Convert())
    {
        res.push_back(e.BuildNDVector());
    }

    return res;
}

std::vector<unsigned int> CentralityCalculator::FlowBetweenness()
{
    NonDominatedVectors<unsigned int> betweenness_scores(this->adjacency_matrix.size());

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

                std::vector<std::vector<Edge>> local_matrix = this->adjacency_matrix;
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
                            betweenness_scores.Update(target, k.capacity);
                        }
                        else if (k.capacity > 0)
                        {
                            k.seen.insert(k.vertex);
                            for (auto & p : local_matrix[k.vertex])
                            {
                                if (k.seen.find(p.target) == k.seen.end())
                                {
                                    unsigned int new_capacity = std::min(k.capacity, p.capacity);
                                    p.capacity -= new_capacity;
                                    q.push({p.target, new_capacity, k.distance + 1, k.seen, k.target});
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

    return betweenness_scores.Convert();
}

std::vector<unsigned int> CentralityCalculator::FlowCloseness()
{
    NonDominatedVectors<unsigned int> closeness_scores(this->adjacency_matrix.size());

    #pragma omp parallel for
    for (size_t v = 0; v < this->adjacency_matrix.size(); v++)
    {
        for (size_t u = v + 1; u < this->adjacency_matrix.size(); u++)
        {
            std::vector<std::vector<Edge>> local_matrix = this->adjacency_matrix;

            unsigned int res = this->GetFlowCloseness(v, u, INT_MAX, std::unordered_set<size_t>(), local_matrix);
            closeness_scores.Update(v, res);
            closeness_scores.Update(u, res);
        }
    }

    return closeness_scores.Convert();
}

unsigned int CentralityCalculator::GetFlowCloseness(
    size_t v, // current vertex
    size_t u, // end point
    unsigned int capacity, 
    std::unordered_set<size_t> seen,
    std::vector<std::vector<Edge>>& local_matrix
)
{
    if (v == u || capacity <= 0)
        return capacity;

    seen.insert(v);

    unsigned int res = 0;
    for (auto & k : local_matrix[v])
    {
        if (seen.find(k.target) == seen.end())
        {
            unsigned int new_capacity = std::min(capacity, k.capacity);
            k.capacity -= new_capacity;
            res += this->GetFlowCloseness(k.target, u, new_capacity, seen, local_matrix);
        }
    }

    return res;
}

std::vector<std::vector<DualCost>> CentralityCalculator::SolveForDistance(const size_t source, const size_t sink)
{
    std::vector<std::vector<DualCost>> res(
        this->adjacency_matrix.size()
    );

    std::queue<std::pair<size_t, unsigned int>> distance_queue;
    distance_queue.push(std::make_pair(source, 0));
    std::unordered_set<size_t> seen;

    while (!distance_queue.empty())
    {
        std::pair<size_t, unsigned int> i = distance_queue.front();
        distance_queue.pop();

        for (const auto & e : this->adjacency_matrix[i.first])
        {
            res[i.first].push_back({e.target, e.capacity, i.second + 1});
            if (seen.find(e.target) == seen.end())
            {
                distance_queue.push(std::make_pair(e.target, i.second + 1));
                seen.insert(e.target);
            }
        }
    }  

    return res;
}


// Edmonds–Karp algorithm
// TODO: There is a bug here, shows up when traversing the network from 1 to 5. Fix this then you are golden
FCVector CentralityCalculator::SolveMaxFlow(
    std::vector<std::vector<Edge>>& local_matrix, 
    size_t source, 
    size_t sink,
    size_t target
)
{
    FCVector label(0,0);
    unsigned int flow = 0;

    while (true)
    {
        std::queue<size_t> q;
        q.push(source);

        std::vector<std::pair<size_t, size_t>> path(local_matrix.size(), std::make_pair(-1,-1)); // pointers to edges in local_matrix
        std::unordered_set<size_t> seen;
        seen.insert(source);

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

        if (seen.find(sink) != seen.end())
        {
            unsigned int path_flow = INT_MAX;
            unsigned int distance = 0;

            for (size_t i = sink; path[i].first != -1; i = local_matrix[path[i].first][path[i].second].parent)
            {
                Edge edge = local_matrix[path[i].first][path[i].second];
                path_flow = std::min(path_flow, edge.capacity - edge.flow);
                distance++; 
            }
 
            for (size_t i = sink; path[i].first != -1; i = local_matrix[path[i].first][path[i].second].parent)
            {                
                Edge& edge = local_matrix[path[i].first][path[i].second];
                edge.flow = edge.flow + path_flow;
            }

            flow = flow + path_flow;
            FCVector temp(path_flow, distance);
            label.Combine(temp);
        }
        else
        {
            if (target == -1)
                return label;
            else 
                return this->SolveMaxFlow(local_matrix, source, sink, -1);
        }
    }
}

// broken, an attempt at a bi-objective network flow algorithm, 
// FCVector CentralityCalculator::SolveMaxFlow(
//     std::vector<std::vector<Edge>> local_matrix, 
//     size_t source, 
//     size_t sink
// )
// {
//     typedef struct cost {
//         size_t vertex;
//         unsigned int distance;
//     } TraversalData;

//     std::stack<TraversalData> modNodes;
//     std::unordered_map<size_t, FCVector> labels;
//     std::unordered_set<size_t> in_stack;

//     for (size_t n = 0; n < local_matrix.size(); n++)
//     {
//         labels.insert({n, FCVector(0,0)});    
//     }

//     modNodes.push({source, 0});
//     in_stack.insert(source);

//     int count = 0;

//     while (!modNodes.empty() && count++ < 10)
//     {
//         TraversalData i = modNodes.top();
//         modNodes.pop();
//         in_stack.erase(i.vertex);

//         labels[i.vertex].DEBUG_Display();

//         for (const auto & j : local_matrix[i.vertex])
//         {
//             FCVector temp(j.capacity, i.distance);
//             FCVector old_label = labels[j.vertex].Copy();
//             FCVector working_label = labels[i.vertex].Copy();

//             // working_label.Combine(temp); // extend, this operation needs to be double checked
//             working_label.Extend(j.capacity, i.distance);
//             labels[j.vertex].Merge(working_label); // merges, this operation needs to be double checked

//             // labels[j.vertex].DEBUG_Display();
            
//             if (old_label.GetFlowCost() != labels[j.vertex].GetFlowCost() && in_stack.find(j.vertex) == in_stack.end())
//             {
//                 modNodes.push({j.vertex, i.distance});
//                 in_stack.insert(j.vertex);
//             }
//         }
//     }

//     return labels[sink];
// }