#include <vector>
#include <iostream>
#include <fstream>
#include <iterator>    
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

    for (auto itr = adjacency_matrix.begin(); itr != adjacency_matrix.end(); itr++)
    {
        std::cout << std::distance(adjacency_matrix.begin(), itr) << ": ";
        for (const auto & a : *itr)
        {
            std::cout << "(" << a.vertex << " " << a.weight << ")" << ", ";
        }
        std::cout << std::endl;
    }

    std::vector<long double> closeness = CentralityCalculator::DEBUG_CalculateCloseness(adjacency_matrix);

    for (auto itr = closeness.begin(); itr != closeness.end(); itr++)
    {
        std::cout << std::distance(closeness.begin(), itr) << ": " << *itr << std::endl;
    }

    
}