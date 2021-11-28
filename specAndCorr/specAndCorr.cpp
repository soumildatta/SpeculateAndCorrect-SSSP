#include "specAndCorr.h"

#include "tSSSPPerformanceCounters.h"

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

	vector<uint32_t> visitCounts(nNodes, 0u);

	while(speculation.size() || correction.size())
	{
		tSSSPPerformanceCounters passPerformanceCounter;

		// Prioritize checking correction buffer
		auto currentNodeIndex { correction.size() ? correction.back() : speculation.back() };
		correction.size() ? correction.pop_back() : speculation.pop_back();
		auto &currentNode { nodes[currentNodeIndex] };

		if(visitCounts[currentNodeIndex]++ >= nNodes)
		{
			throw("Negative Cycle");
		}

		++passPerformanceCounter.nNodesVisited;

		for(auto edgeIndex { 0u }; edgeIndex < currentNode.nEdges; ++edgeIndex)
		{
			auto &currentEdge { edges[currentNode.startEdgeIdx + edgeIndex] };
			auto &distalNodeIndex { currentEdge.distalNodeIdx };
			auto &weight { currentEdge.weight };

			++passPerformanceCounter.nRelaxationAttempts;

			if(nodeCosts[currentNodeIndex].cost != INT32_MAX)
			{
				nodeCost proposedCost(currentNodeIndex, weight + nodeCosts[currentNodeIndex].cost);

				if(proposedCost.cost < nodeCosts[distalNodeIndex].cost)
				{
					if(nodeCosts[distalNodeIndex].cost != INT32_MAX)
					{
						// Node has been relaxed before, put in correction
						correction.emplace_front(distalNodeIndex);
						++passPerformanceCounter.nCorrections;
					}
					else
					{
						// Node has nevver been relaxed before, put in speculation
						speculation.emplace_front(distalNodeIndex);
						++passPerformanceCounter.nSpeculations;
					}

					// Set new weights
					nodeCosts[distalNodeIndex] = proposedCost;

					++passPerformanceCounter.nRelaxations;
				}
				else
				{
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
				// Relaxation cannot occur becuase path to parent is unknown
				cout << "Cannot relax" << endl;
				++performance.nCannotRelax;
			}
		}

		performance += passPerformanceCounter;

	}

	performance.nPasses = 1u;

    return timer.getTime();
}
