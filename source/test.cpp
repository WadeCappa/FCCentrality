#include <vector>
#include <iostream>
#include <fstream>
#include <iterator>    
#include <chrono>

#include "adjacency_builder/adjacency_matrix_builder.h"
#include "flow_cost_centrality/edge_structs.h"
#include "flow_cost_centrality/centrality_calculator.h"

int main(int argc, char* argv[])
{
    std::vector<unsigned int> points_of_interest = {1, 2, 22, 209, 17, 178, 31, 12, 199, 143, 120, 11, 35, 129, 131, 61, 168, 80, 188, 19, 32, 166, 197, 113, 193, 8, 237, 124, 97, 187, 201, 247};

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

    CentralityCalculator calculator(adjacency_matrix);

    // auto start = std::chrono::high_resolution_clock::now();
    // std::vector<std::vector<std::pair<unsigned int, unsigned int>>> closeness = calculator.FlowCostCloseness();
    // auto stop = std::chrono::high_resolution_clock::now();
    // auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    // std::cout << "flow-cost betweenness runtime: " << duration.count() << std::endl;

    // for (size_t i = 0; i < points_of_interest.size(); i++)
    // {
    //     std::cout << "vertex " << points_of_interest[i] << ": " << "(" << closeness[points_of_interest[i]][0].first << "," << closeness[points_of_interest[i]][0].second << ")" << ", ";
    // }

    std::vector<unsigned int> flow_closeness = calculator.FlowCloseness();
    for (size_t i = 0; i < points_of_interest.size(); i++)
    {
        std::cout << "vertex " << points_of_interest[i] << ": " << flow_closeness[points_of_interest[i]]; 
        std::cout << std::endl;
    }
}