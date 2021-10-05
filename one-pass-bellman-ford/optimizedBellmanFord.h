#ifndef OPTIMIZEDBELLMANFORD_H
#define OPTIMIZEDBELLMANFORD_H

// TODO: Includes here 
#include <iostream>
    using std::cout; 
    using std::endl;

#include "tTimer.h"
#include "tGraph.h"

void optimizedBellmanFord(tGraph &graph,  const uint32_t &sourceNode);

#endif
