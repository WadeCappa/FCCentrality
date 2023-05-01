#include <iostream>

#ifndef ADJACENT_H
#define ADJACENT_H

typedef struct edge {
    size_t target;
    size_t parent;
    unsigned int capacity;
    unsigned int flow;
} Edge;

typedef struct dualCost {
    size_t vertex;
    unsigned int weight;
    unsigned int distance;
} DualCost;

#endif