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

    std::cout << "loaded network..." << std::endl;

    CentralityCalculator calculator(adjacency_matrix);

    // flow-cost betweenness
    auto start = std::chrono::high_resolution_clock::now();
    // std::vector<std::vector<std::pair<unsigned int, unsigned int>>> flow_cost_betweenness = calculator.FlowCostBetweenness();
    auto stop = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    // std::cout << "flow-cost betweenness runtime: " << duration.count() << std::endl;

    // std::vector<size_t> s1 = calculator.SortByFlowCostScores(flow_cost_betweenness);
    // for (size_t i = 0; i < output_size && i < s1.size(); i++)
    // {
    //     std::cout << s1[i] << ", ";
    // }

    // std::cout << std::endl;

    // flow betweenness
    // start = std::chrono::high_resolution_clock::now();
    // std::vector<unsigned int> flow_betweenness = calculator.FlowBetweenness();
    // stop = std::chrono::high_resolution_clock::now();

    // duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    // std::cout << "flow betweenneess runtime: " << duration.count() << std::endl;

    // std::vector<size_t> sorted_flow_vertices = calculator.SortByFlowScores(flow_betweenness);
    // for (size_t i = 0; i < output_size && i < sorted_flow_vertices.size(); i++)
    // {
    //     std::cout << sorted_flow_vertices[i] << ", ";
    // }

    // std::cout << std::endl;

    // flow-cost closeness
    start = std::chrono::high_resolution_clock::now();
    std::vector<std::vector<std::pair<unsigned int, unsigned int>>> flow_cost_closeness= calculator.FlowCostCloseness();
    stop = std::chrono::high_resolution_clock::now();

    duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    std::cout << "flow-cost closeness runtime: " << duration.count() << std::endl;

    std::vector<size_t> s3 = calculator.SortByFlowCostScores(flow_cost_closeness);
    for (size_t i = 0; i < output_size && i < s3.size(); i++)
    {
        std::cout << s3[i] << ", ";
    }

    std::cout << std::endl;

    // flow closeness
    start = std::chrono::high_resolution_clock::now();
    std::vector<unsigned int> closeness = calculator.FlowCloseness();
    stop = std::chrono::high_resolution_clock::now();

    duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    std::cout << "flow closeness runtime: " << duration.count() << std::endl;

    std::vector<size_t> s4 = calculator.SortByFlowScores(closeness);
    for (size_t i = 0; i < output_size && i < s4.size(); i++)
    {
        std::cout << s4[i] << ", ";
    }

    std::cout << std::endl;

}