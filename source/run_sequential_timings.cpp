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
    if (argc < 3)
    {
        std::cout << "missing input!" << std::endl;
        exit(1);
    }

    std::ifstream input;
	input.open(argv[1]);

    std::cout << argv[1] << std::endl;

    size_t output_size = atoi(argv[2]);

    if (!input) 
    {
		std::cout << "could not find file" << std::endl;
        exit(1);
	}

    EdgeListBuilder builder;
    builder.Undirected();

    std::vector<std::vector<Edge>> adjacency_matrix = builder.BuildEdgeList(input);
    input.close();

    CentralityCalculator calculator(adjacency_matrix);

    auto start = std::chrono::high_resolution_clock::now();
    std::vector<std::vector<std::pair<unsigned int, unsigned int>>> closeness = calculator.FlowCostBetweenness();
    auto stop = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    // std::cout << "flow-cost betweenness runtime: " << duration.count() << std::endl;
    auto timings = calculator.GetTimings();
    std::cout << duration.count() << ", " << timings.first << ", " << timings.second << std::endl;

    return 1;
}