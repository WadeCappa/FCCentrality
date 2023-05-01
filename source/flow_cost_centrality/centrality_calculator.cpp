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

std::vector<std::vector<std::pair<unsigned int, unsigned int>>> CentralityCalculator::FlowCostCloseness()
{
    NonDominatedVectors<FlowCostLabel> closeness_scores(this->adjacency_matrix.size());
    FlowMaxCalculator calculator;

    #pragma omp parallel for
    for (size_t v = 0; v < this->adjacency_matrix.size(); v++)
    {
        for (size_t u = v + 1; u < this->adjacency_matrix.size(); u++)
        {
            auto copy = this->adjacency_matrix;
            FlowCostLabel result = calculator.SolveFlowCostMax(copy, v, u);
            closeness_scores.Update(v, result);
            closeness_scores.Update(u, result);
        }
    }

    std::vector<std::vector<std::pair<unsigned int, unsigned int>>> res; 

    for (const auto & e : closeness_scores.Convert())
    {
        res.push_back(e.BuildNDVector());
    }

    return res;
}

std::vector<std::vector<std::pair<unsigned int, unsigned int>>> CentralityCalculator::FlowCostBetweenness()
{
    NonDominatedVectors<FlowCostLabel> betweenness_scores(this->adjacency_matrix.size());
    FlowMaxCalculator calculator;

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

                // auto result = this->SolveMaxFlow(copy, source, sink, target);

                FlowCostLabel result = calculator.SolveFlowCostBetweenness(
                    copy,
                    source, 
                    sink,
                    target
                );

                betweenness_scores.Update(target, result);
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
    std::vector<unsigned int> betweenness_scores(this->adjacency_matrix.size(), 0);
    FlowMaxCalculator calculator;

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

                unsigned int result = calculator.SolveFlowBetweenness(
                    copy,
                    source, 
                    sink,
                    target
                );

                betweenness_scores[target] += result;
            }
        }
    }

    return betweenness_scores;
}

std::vector<unsigned int> CentralityCalculator::FlowCloseness()
{
    NonDominatedVectors<unsigned int> closeness_scores(this->adjacency_matrix.size());
    FlowMaxCalculator calculator;

    #pragma omp parallel for
    for (size_t v = 0; v < this->adjacency_matrix.size(); v++)
    {
        for (size_t u = v + 1; u < this->adjacency_matrix.size(); u++)
        {
            std::vector<std::vector<Edge>> local_matrix = this->adjacency_matrix;

            unsigned int res = calculator.SolveFlowMax(local_matrix, v, u);
            closeness_scores.Update(v, res);
            closeness_scores.Update(u, res);
        }
    }

    return closeness_scores.Convert();
}

std::vector<size_t> CentralityCalculator::SortByFlowCostScores(
    const std::vector<std::vector<std::pair<unsigned int, unsigned int>>>& scores
)
{
    typedef struct sortStruct {
        size_t vertex;
        std::vector<std::pair<unsigned int, unsigned int>> flow_cost;
    } SortStruct;

    std::vector<SortStruct> sort_data(scores.size());

    for (size_t i = 0; i < scores.size(); i++)
    {
        sort_data[i].vertex = i;
        sort_data[i].flow_cost = scores[i];
    }

    std::sort(sort_data.begin(), sort_data.end(), [](SortStruct &left, SortStruct &right) {
        if (left.flow_cost.size() != right.flow_cost.size())
        {
            return left.flow_cost.size() < right.flow_cost.size();
        }

        for (size_t i = 0; i < left.flow_cost.size(); i++)
        {
            if (left.flow_cost[i].second != right.flow_cost[i].second)
            {
                return left.flow_cost[i].second < right.flow_cost[i].second;
            }
        }

        return false;
    });

    std::vector<size_t> res(scores.size());

    for (size_t i = 0; i < scores.size(); i++)
    {
        res[i] = sort_data[i].vertex;
    }

    std::reverse(res.begin(), res.end());

    return res;
}


std::vector<size_t> CentralityCalculator::SortByFlowScores(const std::vector<unsigned int>& scores)
{
    std::vector<std::pair<size_t, unsigned int>> vertex_to_flow(scores.size());
    for (size_t i = 0; i < scores.size(); i++)
    {
        vertex_to_flow[i].first = i;
        vertex_to_flow[i].second = scores[i];
    }

    std::sort(vertex_to_flow.begin(), vertex_to_flow.end(), [](auto& left, auto& right){
        return left.second < right.second;
    });

    std::vector<size_t> res(scores.size());
    for (size_t i = 0 ;i < scores.size(); i++)
    {
        res[i] = vertex_to_flow[i].first;
    }

    std::reverse(res.begin(), res.end());

    return res;
}