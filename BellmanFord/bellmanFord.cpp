#include "bellmanFord.h"

double bellmanFord(tGraph &graph, const uint32_t &sourceNode, vector<nodeCost> &nodeCosts, tSSSPPerformanceCounters &performance)
{
    // Structural binding on graph
    auto &[tGraphMemberList] { graph };

    // Initialize and set source node cost to 0 in solution vector
    nodeCosts.clear();
    nodeCosts.resize(nNodes, nodeCost());
    nodeCosts[sourceNode] = nodeCost(sourceNode, 0);

    bool complete { false };

	// Start timer
    tTimer timer;

    // Do N - 1 iterations, or until there are no more relaxations left to do (complete)
    for (auto iteration { 0u }; (iteration < nNodes - 1u) && !complete; ++iteration)
    {
    	complete = true;

    	// Initialize performance counters
    	tSSSPPerformanceCounters passPerformanceCounter;

    	// Go through every proximal node index in the graph
    	for (auto proximalNodeIdx { 0u }; proximalNodeIdx < nNodes; ++proximalNodeIdx)
    	{
    		// Obtain the proximal node from the proximal node index
    		auto &proximalNode { nodes[proximalNodeIdx] };

    		// Go through every edge of the proximal node being processed
    		for (auto edgeIndex { 0u }; edgeIndex < proximalNode.nEdges; ++edgeIndex)
    		{
    			auto &currentEdge { edges[proximalNode.startEdgeIdx + edgeIndex] };
    			auto &distalNodeIdx { currentEdge.distalNodeIdx };
    			auto &weight { currentEdge.weight };
    			auto &proxmialNodeCost { nodeCosts[proximalNodeIdx].cost };

    			++passPerformanceCounter.nRelaxationAttempts;

    			// Make sure that the proximal node has a cost assigned
    			if (proxmialNodeCost != INT32_MAX)
    			{
    				++passPerformanceCounter.nNodesVisited;

    				// Calculate potential new cost with parent node + weight of the edge to child
    				nodeCost proposedCost(proximalNodeIdx, weight + proxmialNodeCost);

    				// Relaxation
    				if (proposedCost.cost < nodeCosts[distalNodeIdx].cost)
    				{
    					// Store proposed cost in solution vector
    					nodeCosts[distalNodeIdx] = proposedCost;

    					// Performed relaxation so do next pass
    					complete = false;

    					++passPerformanceCounter.nRelaxations;
    				}
    				else
    				{
    					// If the proposed cost and the current cost are the same, then take the path with the lower parent node index
    					if((proposedCost.cost == nodeCosts[distalNodeIdx].cost) && (proposedCost.proximalNodeIndex < nodeCosts[distalNodeIdx].proximalNodeIndex))
    					{
    						nodeCosts[distalNodeIdx] = proposedCost;
    						++passPerformanceCounter.nBetterParentIndex;
    					}

    					++passPerformanceCounter.nAlreadyOptimal;
    				}
    			}
    			else
    			{
    				++passPerformanceCounter.nCannotRelax;
    			}
    		}
    	}

    	performance += passPerformanceCounter;
    	++performance.nPasses;
    }

    return timer.getTime();
}
