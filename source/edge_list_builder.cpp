#include "edge_list_builder.h"

EdgeListBuilder::EdgeListBuilder()
{
    this->undirected = false;
}

void EdgeListBuilder::AddEdge(
    std::vector<std::vector<Adjacent>>& list, 
    const size_t start, 
    const size_t destination, 
    const double weight
)
{
    if (list.size() <= start)
    {
        list.resize(start + 1);
    }

    list[start].push_back({destination, weight});
}

std::vector<std::vector<Adjacent>> EdgeListBuilder::BuildEdgeList(std::ifstream& input_stream)
{
    std::vector<std::vector<Adjacent>> adjacency_list;
    std::string line;

    for(size_t vertex_id = 0; std::getline(input_stream, line); vertex_id++)
    {
        std::istringstream input(line);

        size_t start;
        size_t stop;
        input >> start;
        input >> stop;
        double value = 1;
        input >> value;

        this->AddEdge(adjacency_list, start, stop, value);

        if (this->undirected) 
        {
            this->AddEdge(adjacency_list, stop, start, value);
        }
    }

    return adjacency_list;
}

void EdgeListBuilder::Undirected()
{
    this->undirected = true;
}