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
    std::vector<long double> closeness = calculator.FlowCloseness();
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    std::cout << duration.count() << std::endl;

    size_t bestVertex = 0;
    long double bestScore = 0;

    for (auto itr = closeness.begin(); itr != closeness.end(); itr++)
    {
        if (*itr > bestScore )
        {
            bestVertex = std::distance(closeness.begin(), itr);
            bestScore = *itr;
        }
        std::cout << std::distance(closeness.begin(), itr) << ": " << *itr << std::endl;
    }

    std::cout << bestVertex << " -> " << bestScore << std::endl;
}