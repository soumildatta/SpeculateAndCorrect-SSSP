#include "specAndCorr.h"

double specAndCorr(tGraph &graph, const uint32_t &sourceNode, vector<nodeCost> &nodeCosts, tSSSPPerformanceCounters &performance)
{
	tTimer timer;

	auto &[tGraphMemberList] { graph };

	list<uint32_t> speculation, correction;
	// Add source node to speculation
	speculation.emplace_front(sourceNode);

	nodeCosts.clear();
	nodeCosts.resize(nNodes, nodeCost());
	nodeCosts[sourceNode] = nodeCost(sourceNode, 0);

	// Keep track of negative edge loops
	vector<uint32_t> visitCounts(nNodes, 0u);

	while(speculation.size() || correction.size())
	{
		tSSSPPerformanceCounters passPerformanceCounter;

		// Prioritize checking correction buffer
		auto currentNodeIndex { correction.size() ? correction.back() : speculation.back() };
		correction.size() ? correction.pop_back() : speculation.pop_back();
		auto &currentNode { nodes[currentNodeIndex] };

		// Negative edge loop detected
		if(visitCounts[currentNodeIndex]++ >= nNodes)
		{
			throw("Negative Cycle");
		}

		++passPerformanceCounter.nNodesVisited;

		// Go through every edge of the current node being processed
		for(auto edgeIndex { 0u }; edgeIndex < currentNode.nEdges; ++edgeIndex)
		{
			auto &currentEdge { edges[currentNode.startEdgeIdx + edgeIndex] };
			auto &distalNodeIndex { currentEdge.distalNodeIdx };
			auto &weight { currentEdge.weight };

			++passPerformanceCounter.nRelaxationAttempts;

			// Make sure that the parent node has alreayd been relaxed
			if(nodeCosts[currentNodeIndex].cost != INT32_MAX)
			{
				// Potential better cost
				nodeCost proposedCost(currentNodeIndex, weight + nodeCosts[currentNodeIndex].cost);

				// Check to see if the proposed cost is better than the current cost
				if(proposedCost.cost < nodeCosts[distalNodeIndex].cost)
				{
					if(nodeCosts[distalNodeIndex].cost != INT32_MAX)
					{
						// Node has been relaxed before, put in correction buffer
						correction.emplace_front(distalNodeIndex);
						++passPerformanceCounter.nCorrections;
					}
					else
					{
						// Node has nevver been relaxed before, put in speculation buffer
						speculation.emplace_front(distalNodeIndex);
						++passPerformanceCounter.nSpeculations;
					}

					// Set new weights
					nodeCosts[distalNodeIndex] = proposedCost;

					++passPerformanceCounter.nRelaxations;
				}
				else
				{
					// Check to see if proposed cost is the same as current cost
					// If it is, then choose the path with the lower parent node index
					if ((proposedCost.cost == nodeCosts[distalNodeIndex].cost) &&
						(proposedCost.proximalNodeIndex < nodeCosts[distalNodeIndex].proximalNodeIndex))
					{
						nodeCosts[distalNodeIndex].proximalNodeIndex = currentNodeIndex;
						++passPerformanceCounter.nBetterParentIndex;
					}

					++passPerformanceCounter.nAlreadyOptimal;
				}
			}
			else
			{
				// Relaxation cannot occur because path to parent is unknown -- should be impossible during generational graph traversal
				cout << "Cannot relax" << endl;
				++performance.nCannotRelax;
			}
		}

		performance += passPerformanceCounter;

	}

	performance.nPasses = 1u;

    return timer.getTime();
}
