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

struct tData {
	vector<tCSRNode> nodes;
	vector<tCSREdge> edges;

	vector<nodeCost> solution;

	tPool speculationPool;
	tPool correctionPool;

	uint32_t source;

#define tDataMemberList nodes, edges, solution, speculationPool, correctionPool, source
};

#endif /* TDATA_H */
