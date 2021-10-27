/*
 * validation.cpp
 *
 *  Created on: Oct 27, 2021
 *      Author: sdatta
 */

#include "validation.h"

#include <fstream>
using std::ifstream;

#include <iostream>
    using std::cout;
    using std::endl;

#include "tGraph.h"

template<class T>
inline bool operator==(const vector<T> &lhs, const vector<T> &rhs)
{
	return (lhs.size()*sizeof(T) == rhs.size()*sizeof(T)) ? memcmp(lhs.data(), rhs.data(), sizeof(T) * lhs.size()) == 0 : false;
}

template<class T>
inline bool operator!=(const vector<T> &lhs, const vector<T> &rhs)
{
	return !(lhs == rhs);
}

template<class T1, class T2>
inline bool operator==(const vector<T1> &lhs, const vector<T2> &rhs)
{
	return (lhs.size()*sizeof(T1) == rhs.size()*sizeof(T2)) ? memcmp(lhs.data(), rhs.data(), sizeof(T1) * lhs.size()) == 0 : false;
}

template<class T1, class T2>
inline bool operator!=(const vector<T1> &lhs, const vector<T2> &rhs)
{
	return !(lhs == rhs);
}

bool readSolution(const path &filepath, vector<nodeCost> &nodeCosts)
{
	char *temp;
	vector<nodeCost> parentNodeCosts;

	ifstream ifs { filepath };

	if (!ifs)
	{
		cout << "Error reading file at " << filepath << endl;
		exit(0);
	}

	size_t nSolutions;

//	ifs.read(temp, 1);
//	ifs.read(temp, sizeof(double));
//	ifs.read(temp, sizeof(uint32_t));
//	ifs.read(temp, 1);
//	ifs.read(temp, 72);
	ifs.read(temp, sizeof(nSolutions));
	parentNodeCosts.resize(nSolutions);
	ifs.read((char *) parentNodeCosts.data(), sizeof(nodeCost) * nSolutions);

	return true;
}
