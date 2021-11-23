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
	uint32_t attemptedRelaxations = 0u;
	uint32_t nodesVisited;
	uint32_t numRelaxations;
	uint32_t numBetterParentsFound;
	uint32_t numOptimalPathsAttempted;
	uint32_t numCannotRelax;
	uint32_t numPasses;

	inline void printMetrics()
	{
		cout << "Number of Attempted Relaxations: " << attemptedRelaxations << endl;
		cout << "Number of nodes visited: " << nodesVisited << endl;
		cout << "Number of relaxations: " << numRelaxations << endl;
		cout << "Number of better parents found: " << numBetterParentsFound << endl;
		cout << "Number of Optimal Paths Attempted: " << numOptimalPathsAttempted << endl;
		cout << "Number of times could not relax: " << numCannotRelax << endl;
		cout << "Number of passes: " << numPasses << endl;
 	}
};

#endif /* PERFORMANCEMETRICS_H */
