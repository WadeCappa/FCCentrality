#include "data_extractor.h"

int main()
{
    std::ifstream input;
	input.open("../networks/karate.txt");

    if (!input) 
    {
		std::cout << "could not find file" << std::endl;
        exit(1);
	}

    std::vector<std::vector<unsigned int>> edge_list = DataExtractor::BuildEdgeList(input);
    input.close();
}