#ifndef TDATA_H
#define TDATA_H

#include "tPool.h"
#include "tGraph.h"

#include <vector>
using std::vector;

#include <atomic>
using std::atomic_bool;
using std::atomic_uint64_t;

struct tData {
	vector<tCSRNode> nodes;
	vector<tCSREdge> edges;

	vector<nodeCost> solution;

	vector<uint32_t> nodeVisitCounts;

	tPool speculationPool;
	tPool correctionPool;

	uint64_t nIncompleteTasks;
	uint32_t source;
	uint32_t nNodes;
	uint32_t maxThreads;

	bool abortFlag;

#define tDataMemberList nodes, edges, solution, nodeVisitCounts, speculationPool, correctionPool, nIncompleteTasks, source, nNodes, maxThreads, abortFlag
};

#endif /* TDATA_H */
