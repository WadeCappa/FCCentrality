#include <vector>
#include <iostream>
#include <fstream>
#include <iterator>    
#include "edge_list_builder.h"

int main()
{
    std::ifstream input;
	input.open("../networks/karate.txt");

    if (!input) 
    {
		std::cout << "could not find file" << std::endl;
        exit(1);
	}

    EdgeListBuilder builder;
    builder.Undirected();

    std::vector<std::vector<Adjacent>> edge_list = builder.BuildEdgeList(input);
    input.close();

    for (auto itr = edge_list.begin(); itr != edge_list.end(); itr++)
    {
        std::cout << std::distance(edge_list.begin(), itr) << ": ";
        for (const auto & a : *itr)
        {
            std::cout << "(" << a.vertex << " " << a.weight << ")" << ", ";
        }
        std::cout << std::endl;
    }
}