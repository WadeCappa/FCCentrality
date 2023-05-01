#include <vector>
#include <iostream>
#include <fstream>
#include <iterator>    
#include <chrono>

#include "adjacency_matrix_builder.h"
#include "adjacent.h"
#include "centrality_calculator.h"

int main(int argc, char* argv[])
{
    std::ifstream input;
	input.open(argv[1]);

    if (!input) 
    {
		std::cout << "could not find file" << std::endl;
        exit(1);
	}

    EdgeListBuilder builder;
    builder.Undirected();

    std::vector<std::vector<Edge>> adjacency_matrix = builder.BuildEdgeList(input);
    input.close();

    std::cout << "loaded network..." << std::endl;

    for (size_t v = 0 ; v < adjacency_matrix.size(); v++)
    {
        for (const auto & e : adjacency_matrix[v])
        {
            std::cout << v << " -> " << e.target << ", " << e.capacity << ", " << e.flow << ", " << e.parent << std::endl;
        }
    }

    CentralityCalculator calculator(adjacency_matrix);

    auto start = std::chrono::high_resolution_clock::now();
    std::vector<unsigned int> flow_betweenness = calculator.FlowBetweenness();
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    std::cout << "flow betweenneess runtime: " << duration.count() << std::endl;

    for (size_t i = 0; i < flow_betweenness.size(); i++)
    {
        std::cout << i << ": " << flow_betweenness[i] << std::endl;
    }

    start = std::chrono::high_resolution_clock::now();
    std::vector<std::vector<std::pair<unsigned int, unsigned int>>> closeness = calculator.FlowCostBetweenness();
    stop = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    std::cout << "flow-cost betweenness runtime: " << duration.count() << std::endl;

    for (auto itr = closeness.begin(); itr != closeness.end(); itr++)
    {
        std::cout << "vertex " << std::distance(closeness.begin(), itr) << ": ";
        for (const auto & e : *itr)
        {
             std::cout << "(" << e.first << "," << e.second << ")" << ", ";
        }        
        std::cout << std::endl;
    }
}