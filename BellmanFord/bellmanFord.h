#ifndef BELLMANFORD_H
#define BELLMANFORD_H

#include <iostream>
    using std::cout; 
    using std::endl;

#include "tTimer.h"
#include "tGraph.h"
#include "tSSSPPerformanceCounters.h"

double bellmanFord(tGraph &graph, const uint32_t &sourceNode, vector<nodeCost> &nodeCosts, tSSSPPerformanceCounters &performance);

#endif
