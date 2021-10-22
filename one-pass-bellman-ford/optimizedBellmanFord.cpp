#include "optimizedBellmanFord.h"

struct nodeCost
{
	uint32_t proximalNodeIndex;
	uint32_t cost;

	nodeCost(void) : proximalNodeIndex(~0u), cost(INT32_MAX) { return; }
	nodeCost(uint32_t _proximalNodeIndex, int32_t _cost) : proximalNodeIndex(_proximalNodeIndex), cost(_cost) { return; }
	~nodeCost() { return; }
};

double optimizedBellmanFord(tGraph &graph, const uint32_t &sourceNode)
{
	tTimer timer;

	cout << "Performing Optimized Bellman Ford" << endl;

	auto &[tGraphMemberList] { graph };

	list<uint32_t> speculation;
	list<uint32_t> correction;

	// Add source node to speculation
	speculation.emplace_back(sourceNode);

//	list<uint32_t> costs(graph.nNodes, INT32_MAX), nodeVisitCounts(graph.nNodes, INT32_MAX);

	vector<nodeCost> nodeCosts;
	nodeCosts.clear();
	nodeCosts.resize(nNodes, nodeCost());
	nodeCosts[sourceNode] = nodeCost(sourceNode, 0);

	while(speculation.size() || correction.size())
	{
		auto &currentNodeIndex { correction.size() ? correction.back() : speculation.back() };
		correction.size() ? correction.pop_back() : speculation.pop_back();
		auto &currentNode { nodes[currentNodeIndex] };

		for(auto edgeIndex { 0u }; edgeIndex < currentNode.nEdges; ++edgeIndex)
		{
			auto &currentEdge { edges[currentNode.startEdgeIdx + edgeIndex] };
			auto &distalNodeIndex { currentEdge.distalNodeIdx };
			auto &weight { currentEdge.weight };
			auto &proximalNodeCost { nodeCosts[currentNodeIndex].cost };

			if(currentNodeIndex != INT32_MAX)
			{
				nodeCost proposedCost(proximalNodeIdx, weight + proxmialNodeCost);

				if(nodeCost[distalNodeIndex] == INT32_MAX)
				{
					nodeCost[distalNodeIndex] = proposedCost;
					speculation.emplace_back(distalNodeIndex);
				}
//				else
//				{
////					nodeCost[distalNodeIndex] = min()
//				}
			}
		}

	}


    return timer.getTime();
}
