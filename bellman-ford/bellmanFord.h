#ifndef BELLMANFORD_H
#define BELLMANFORD_H

// TODO: Includes here 
#include <iostream>
    using std::cout; 
    using std::endl;

#include "tTimer.h"
#include "tGraph.h"

double bellmanFord(tGraph &graph, const uint32_t &sourceNode);

#endif
