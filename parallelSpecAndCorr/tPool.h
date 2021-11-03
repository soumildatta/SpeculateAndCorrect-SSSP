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
	uint32_t *pool;
	tIndex removeIndex;
	tIndex addIndex;
	tIndex bufferSize;

#define tPoolMemberList pool, removeIndex, addIndex, bufferSize
};

#endif /* TPOOL_H */
