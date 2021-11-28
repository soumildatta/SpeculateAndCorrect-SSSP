/*
 * tData.h
 *
 *  Created on: Nov 3, 2021
 *      Author: sdatta
 */

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

	tPool pool;

	uint64_t nPasses;
	uint64_t nIncompleteTasks;
	uint32_t nNodes;
	uint32_t source;

	bool abortFlag;

#define tDataMemberList nodes, edges, solution, speculationPool, correctionPool, nPasses, nIncompleteTasks, nNodes, source, abortFlag
};

#endif /* TDATA_H */
