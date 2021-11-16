/*
 * tPool.h
 *
 *  Created on: Nov 3, 2021
 *      Author: sdatta
 */

#ifndef TPOOL_H
#define TPOOL_H

#include "UtilityFunctions.h"

#include <atomic>
using std::atomic_uint64_t;

struct tPool {
	// pool buffer
	vector<atomic_uint64_t> pool;
	atomic_uint64_t removeIndex;
	atomic_uint64_t addIndex;
	tIndex bufferSize;

#define tPoolMemberList pool, removeIndex, addIndex, bufferSize
};

#endif /* TPOOL_H */
