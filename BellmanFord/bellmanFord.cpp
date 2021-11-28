#include "bellmanFord.h"

double bellmanFord(tGraph &graph, const uint32_t &sourceNode, vector<nodeCost> &nodeCosts, tSSSPPerformanceCounters &performance)
{
    tTimer timer;

    auto &[tGraphMemberList] { graph };

    // Vector to keep track of proximal node cproximalNodeCostosts
    nodeCosts.clear();
    nodeCosts.resize(nNodes, nodeCost());
    nodeCosts[sourceNode] = nodeCost(sourceNode, 0);

    bool complete { false };

    for (auto iteration { 0u }; (iteration < nNodes - 1u) && !complete; ++iteration)
    {
    	complete = true;
    	tSSSPPerformanceCounters passPerformanceCounter;

    	for (auto proximalNodeIdx { 0u }; proximalNodeIdx < nNodes; ++proximalNodeIdx)
    	{
    		auto &proximalNode { nodes[proximalNodeIdx] };

    		for (auto edgeIndex { 0u }; edgeIndex < proximalNode.nEdges; ++edgeIndex)
    		{
    			auto &currentEdge { edges[proximalNode.startEdgeIdx + edgeIndex] };
    			auto &distalNodeIdx { currentEdge.distalNodeIdx };
    			auto &weight { currentEdge.weight };
    			auto &proxmialNodeCost { nodeCosts[proximalNodeIdx].cost };

    			++passPerformanceCounter.nRelaxationAttempts;

    			if (proximalNodeIdx != INT32_MAX)
    			{
    				++passPerformanceCounter.nNodesVisited;
    				nodeCost proposedCost(proximalNodeIdx, weight + proxmialNodeCost);

    				// Relaxation
    				if (proposedCost.cost < nodeCosts[distalNodeIdx].cost)
    				{
    					nodeCosts[distalNodeIdx] = proposedCost;
    					// Performed relaxation so do next pass
    					complete = false;

    					++passPerformanceCounter.nRelaxations;
    				}
    				else
    				{
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
