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

    std::cout << "loaded network..." << std::endl;

    CentralityCalculator calculator(adjacency_matrix);

    auto start = std::chrono::high_resolution_clock::now();
    std::vector<unsigned int> flow_betweenness = calculator.FlowBetweenness();
    auto stop = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    std::cout << "flow betweenneess runtime: " << duration.count() << std::endl;

    std::vector<size_t> sorted_flow_vertices = calculator.SortByFlowScores(flow_betweenness);
    for (size_t i = 0; i < output_size && i < sorted_flow_vertices.size(); i++)
    {
        std::cout << sorted_flow_vertices[i] << ", ";
    }

    std::cout << std::endl;

    // for (size_t i = 0; i < flow_betweenness.size(); i++)
    // {
    //     std::cout << i << ": " << flow_betweenness[i] << std::endl;
    // }

    start = std::chrono::high_resolution_clock::now();
    std::vector<std::vector<std::pair<unsigned int, unsigned int>>> closeness = calculator.FlowCostBetweenness();
    stop = std::chrono::high_resolution_clock::now();

    duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    std::cout << "flow-cost betweenness runtime: " << duration.count() << std::endl;

    std::vector<size_t> sorted_vertices = calculator.SortByFlowCostScores(closeness);
    for (size_t i = 0; i < output_size && i < sorted_vertices.size(); i++)
    {
        std::cout << sorted_vertices[i] << ", ";
    }

    std::cout << std::endl;

}