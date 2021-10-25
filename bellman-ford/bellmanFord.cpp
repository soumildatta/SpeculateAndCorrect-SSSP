#include "bellmanFord.h"

struct nodeCost
{
	uint32_t proximalNodeIndex;
	uint32_t cost;

	nodeCost(void) : proximalNodeIndex(~0u), cost(INT32_MAX) { return; }
	nodeCost(uint32_t _proximalNodeIndex, int32_t _cost) : proximalNodeIndex(_proximalNodeIndex), cost(_cost) { return; }
	~nodeCost() { return; }
};

double bellmanFord(tGraph &graph, const uint32_t &sourceNode)
{
    tTimer timer;

    auto &[tGraphMemberList] { graph };

    // Vector to keep track of proximal node cproximalNodeCostosts
    vector<nodeCost> nodeCosts;
    nodeCosts.clear();
    nodeCosts.resize(nNodes, nodeCost());
    nodeCosts[sourceNode] = nodeCost(sourceNode, 0);

    bool complete { false };

    for (auto iteration { 0u }; (iteration < nNodes - 1u) && !complete; ++iteration)
    {
    	complete = true;

    	for (auto proximalNodeIdx { 0u }; proximalNodeIdx < nNodes; ++proximalNodeIdx)
    	{
    		auto &proximalNode { nodes[proximalNodeIdx] };

    		for (auto edgeIndex { 0u }; edgeIndex < proximalNode.nEdges; ++edgeIndex)
    		{
    			auto &currentEdge { edges[proximalNode.startEdgeIdx + edgeIndex] };
    			auto &distalNodeIdx { currentEdge.distalNodeIdx };
    			auto &weight { currentEdge.weight };
    			auto &proxmialNodeCost { nodeCosts[proximalNodeIdx].cost };

    			if (proximalNodeIdx != INT32_MAX)
    			{
    				nodeCost proposedCost(proximalNodeIdx, weight + proxmialNodeCost);

    				// Relaxation
    				if (proposedCost.cost < nodeCosts[distalNodeIdx].cost)
    				{
    					nodeCosts[distalNodeIdx] = proposedCost;
    					// Performed relaxation so do next pass
    					complete = false;
    				}
    				else
    				{
    					if((proposedCost.cost == nodeCosts[distalNodeIdx].cost) && (proposedCost.proximalNodeIndex < nodeCosts[distalNodeIdx].proximalNodeIndex))
    					{
    						nodeCosts[distalNodeIdx] = proposedCost;
    					}
    				}
    			}
    		}
    	}
    }

    return timer.getTime();
}
