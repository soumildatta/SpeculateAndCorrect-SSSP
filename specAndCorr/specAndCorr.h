#ifndef OPTIMIZEDBELLMANFORD_H
#define OPTIMIZEDBELLMANFORD_H

#include <iostream>
    using std::cout; 
    using std::endl;

#include <list>
    using std::list;

#include <algorithm>
    using std::min;

#include "tTimer.h"
#include "tGraph.h"
#include "tSSSPPerformanceCounters.h"

double specAndCorr(tGraph &graph, const uint32_t &sourceNode, vector<nodeCost> &nodeCosts, tSSSPPerformanceCounters &performance);

#endif
