#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include "adjacent.h"

#ifndef DATAEXTRACTOR_H
#define DATAEXTRACTOR_H

class EdgeListBuilder // TODO: Should be called edge_list builder, should account for undirected vs directed, weighted vs unweighted
{
    private:
        bool undirected;
        void AddEdge(std::vector<std::vector<Edge>>& list, const size_t start, const size_t destination, const unsigned int weight);

    public:
        EdgeListBuilder();
        std::vector<std::vector<Edge>> BuildEdgeList(std::ifstream& input_stream);
        void Undirected();
};

#endif