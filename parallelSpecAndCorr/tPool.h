/*
 * tPool.h
 *
 *  Created on: Nov 3, 2021
 *      Author: sdatta
 */

#ifndef TPOOL_H
#define TPOOL_H

#include "UtilityFunctions.h"

struct tPool {
	// pool buffer
	vector<uint64_t> pool;
	uint64_t removeIndex;
	uint64_t addIndex;
	tIndex bufferSize;

#define tPoolMemberList pool, removeIndex, addIndex, bufferSize
};

#endif /* TPOOL_H */
