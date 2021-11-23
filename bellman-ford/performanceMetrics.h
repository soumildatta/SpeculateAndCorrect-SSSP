/*
 * performanceMetrics.h
 *
 *  Created on: Nov 22, 2021
 *      Author: soumildatta
 */

#ifndef PERFORMANCEMETRICS_H
#define PERFORMANCEMETRICS_H

#include <stdlib.h>

struct performanceMetrics {
	uint32_t attemptedRelaxations;
	uint32_t nodesVisited;
	uint32_t numRelaxations;
	uint32_t numBetterParentsFound;
	uint32_t numOptimalPathsAttempted;
	uint32_t numCannotRelax;
	uint32_t numPasses;
};

#endif /* PERFORMANCEMETRICS_H */
