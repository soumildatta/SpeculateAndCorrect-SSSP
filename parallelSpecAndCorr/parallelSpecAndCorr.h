/*
 * parallelSpecAndCorr.h
 *
 *  Created on: Oct 25, 2021
 *      Author: soumildatta
 */

#ifndef PARALLELSPECANDCORR_H
#define PARALLELSPECANDCORR_H

#include <iostream>
    using std::cout;
    using std::endl;

#include "tTimer.h"
#include "tGraph.h"

double parallelSpecAndCorr(tGraph &graph, const uint32_t &sourceNode);

#endif /* PARALLELSPECANDCORR_H */
