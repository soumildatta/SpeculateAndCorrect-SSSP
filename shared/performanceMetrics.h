/*
 * performanceMetrics.h
 *
 *  Created on: Nov 22, 2021
 *      Author: soumildatta
 */

#ifndef PERFORMANCEMETRICS_H
#define PERFORMANCEMETRICS_H

#include <stdlib.h>
#include "UtilityFunctions.h"

struct performanceMetrics {
	tPerfCounter attemptedRelaxations;
	tPerfCounter nodesVisited;
	tPerfCounter numRelaxations;
	tPerfCounter numBetterParentsFound;
	tPerfCounter numOptimalPathsAttempted;
	tPerfCounter numCannotRelax;
	tPerfCounter numPasses;
	tPerfCounter numCorrections;
	tPerfCounter numSpeculations;

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
