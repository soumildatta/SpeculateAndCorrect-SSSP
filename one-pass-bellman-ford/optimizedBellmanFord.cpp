#include "optimizedBellmanFord.h"

//struct nodeCost
//{
//	uint32_t proximalNodeIndex;
//	uint32_t cost;
//
//	nodeCost(void) : proximalNodeIndex(~0u), cost(INT32_MAX) { return; }
//	nodeCost(uint32_t _proximalNodeIndex, int32_t _cost) : proximalNodeIndex(_proximalNodeIndex), cost(_cost) { return; }
//	~nodeCost() { return; }
//};

double optimizedBellmanFord(tGraph &graph, const uint32_t &sourceNode)
{
	tTimer timer;

	auto &[tGraphMemberList] { graph };

	list<uint32_t> speculation, correction;
	// Add source node to speculation
	speculation.emplace_front(sourceNode);

	vector<nodeCost> nodeCosts;
	nodeCosts.clear();
	nodeCosts.resize(nNodes, nodeCost());
	nodeCosts[sourceNode] = nodeCost(sourceNode, 0);

	while(speculation.size() || correction.size())
	{
		// Prioritize checking correction buffer
		auto currentNodeIndex { correction.size() ? correction.back() : speculation.back() };
		correction.size() ? correction.pop_back() : speculation.pop_back();
		auto &currentNode { nodes[currentNodeIndex] };

		for(auto edgeIndex { 0u }; edgeIndex < currentNode.nEdges; ++edgeIndex)
		{
			auto &currentEdge { edges[currentNode.startEdgeIdx + edgeIndex] };
			auto &distalNodeIndex { currentEdge.distalNodeIdx };
			auto &weight { currentEdge.weight };

			if(nodeCosts[currentNodeIndex].cost != INT32_MAX)
			{
				nodeCost proposedCost(currentNodeIndex, weight + nodeCosts[currentNodeIndex].cost);

				if(proposedCost.cost < nodeCosts[distalNodeIndex].cost)
				{
					if(nodeCosts[distalNodeIndex].cost != INT32_MAX)
					{
						// Node has been relaxed before, put in correction
						correction.emplace_front(distalNodeIndex);
					}
					else
					{
						// Node has nevver been relaxed before, put in speculation
						speculation.emplace_front(distalNodeIndex);
					}

					// Set new weights
					nodeCosts[distalNodeIndex] = proposedCost;
				}
				else
				{
					if ((proposedCost.cost == nodeCosts[distalNodeIndex].cost) &&
						(proposedCost.proximalNodeIndex < nodeCosts[distalNodeIndex].proximalNodeIndex))
					{
						nodeCosts[distalNodeIndex].proximalNodeIndex = currentNodeIndex;
					}
				}
			}
			else
			{
				// Relaxation cannot occur becuase path to parent is unknown
				cout << "Cannot relax" << endl;
			}
		}

	}


    return timer.getTime();
}
