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

	list<uint32_t> speculation, correction;
//	list<uint32_t> costs(graph.nNodes, INT32_MAX), nodeVisitCounts(graph.nNodes, INT32_MAX);

	vector<nodeCost> nodeCosts;
	nodeCosts.clear();
	nodeCosts.resize(nNodes, nodeCost());
	nodeCosts[sourceNode] = nodeCost(sourceNode, 0);

//
	while(speculation.size() || correction.size())
	{
		auto &currentNode { correction.pop_back() };
//		tCSRNode
	}


    return timer.getTime();
}
