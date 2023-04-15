#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

#ifndef DATAEXTRACTOR_H
#define DATAEXTRACTOR_H

typedef struct adjacent {
    size_t vertex;
    double weight;
} Adjacent;

class EdgeListBuilder // TODO: Should be called edge_list builder, should account for undirected vs directed, weighted vs unweighted
{
    private:
        bool undirected;
        void AddEdge(std::vector<std::vector<Adjacent>>& list, const size_t start, const size_t destination, const double weight);

    public:
        EdgeListBuilder();
        std::vector<std::vector<Adjacent>> BuildEdgeList(std::ifstream& input_stream);
        void Undirected();
};

#endif