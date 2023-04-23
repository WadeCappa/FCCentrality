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

    std::vector<std::vector<Adjacent>> adjacency_matrix = builder.BuildEdgeList(input);
    input.close();

    std::cout << "loaded network..." << std::endl;

    // for (auto itr = adjacency_matrix.begin(); itr != adjacency_matrix.end(); itr++)
    // {
    //     std::cout << std::distance(adjacency_matrix.begin(), itr) << ": ";
    //     for (const auto & a : *itr)
    //     {
    //         std::cout << "(" << a.vertex << " " << a.weight << ")" << ", ";
    //     }
    //     std::cout << std::endl;
    // }

    auto start = std::chrono::high_resolution_clock::now();
    CentralityCalculator calculator(adjacency_matrix);
    std::vector<std::vector<std::pair<unsigned int, unsigned int>>> closeness = calculator.FC_Betweenness();
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    std::cout << duration.count() << std::endl;


    for (auto itr = closeness.begin(); itr != closeness.end(); itr++)
    {
        std::cout << "vertex " << std::distance(closeness.begin(), itr) << ": ";
        for (const auto & e : *itr)
        {
             std::cout << "(" << e.first << "," << e.second << ")" << ", ";
        }        
        std::cout << std::endl;
    }


    std::vector<unsigned int> flow_betweenness = calculator.FlowBetweenness();
    for (size_t i = 0; i < flow_betweenness.size(); i++)
    {
        std::cout << i << ": " << flow_betweenness[i] << std::endl;
    }
}