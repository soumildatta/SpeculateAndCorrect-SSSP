#ifndef BELLMANFORD_H
#define BELLMANFORD_H

#include <iostream>
    using std::cout; 
    using std::endl;

#include "tTimer.h"
#include "tGraph.h"
#include "performanceMetrics.h"

double bellmanFord(tGraph &graph, const uint32_t &sourceNode, vector<nodeCost> &nodeCosts, performanceMetrics performance);

#endif
