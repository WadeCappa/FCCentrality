#include <vector>
#include <iostream>
#include <fstream>

#ifndef DATAEXTRACTOR_H
#define DATAEXTRACTOR_H

class DataExtractor
{
    public:
        static std::vector<std::vector<unsigned int>> BuildEdgeList(std::ifstream& input_stream);
};

#endif