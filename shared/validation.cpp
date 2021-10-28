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
#include "UtilityFunctions.h"

enum eSSSPSolutionType : uint8_t
{
    noSolution = 0u,
    eSpecAndCorr,
    eBellmanFord,
    henrys,
};

typedef unsigned long long tPerfCounter;

struct tSSSPPerformanceCounters
{
	tPerfCounter nNodesVisited;
    tPerfCounter nAlreadyOptimal;
    tPerfCounter nBetterParentIndex;
    tPerfCounter nCannotRelax;
    tPerfCounter nRelaxationAttempts;
    tPerfCounter nRelaxations;
    tPerfCounter nCorrections;
    tPerfCounter nSpeculations;
    tPerfCounter nPasses;

#define tSSSPPerformanceCountersMemberList nNodesVisited, nAlreadyOptimal, nBetterParentIndex, nCannotRelax, nRelaxationAttempts, nRelaxations, nCorrections, nSpeculations, nPasses
#define dumptSSSPPerformanceCounters dumpList(nNodesVisited, nAlreadyOptimal, nBetterParentIndex, nCannotRelax, nRelaxationAttempts, nRelaxations, nCorrections, nSpeculations, nPasses)

    inline tSSSPPerformanceCounters(void)
    {
        clear();
    }

    inline void clear(void)
    {
    	nNodesVisited = 0u;
        nAlreadyOptimal = 0u;
        nBetterParentIndex = 0u;
        nCannotRelax = 0u;
        nRelaxationAttempts = 0u;
        nRelaxations = 0u;
        nCorrections = 0u;
        nSpeculations = 0u;
        nPasses = 0u;
        return;
    }

	inline tSSSPPerformanceCounters &operator+=(const tSSSPPerformanceCounters &rhs)
	{
		nNodesVisited += rhs.nNodesVisited;
		nAlreadyOptimal += rhs.nAlreadyOptimal;
		nBetterParentIndex += rhs.nBetterParentIndex;
		nCannotRelax += rhs.nCannotRelax;
		nRelaxationAttempts += rhs.nRelaxationAttempts;
		nRelaxations += rhs.nRelaxations;
		nCorrections += rhs.nCorrections;
		nSpeculations += rhs.nSpeculations;

		return *this;
	}
};

bool readSolution(const path &filepath, vector<nodeCost> &nodeCosts)
{
	tSSSPPerformanceCounters sumPerformanceCounter;
	uint32_t sourceNode;
	eSSSPSolutionType lastUsedMethod;
	double solutionTime;

//	char *temp;
	vector<nodeCost> parentNodeCosts;

	ifstream ifs { filepath };

	if (!ifs)
	{
		cout << "Error reading file at " << filepath << endl;
		exit(0);
	}

	size_t nSolutions;

	ifs.read((char *) &lastUsedMethod, sizeof(lastUsedMethod));
	ifs.read((char *) &solutionTime, sizeof(solutionTime));
	ifs.read((char *) &sourceNode, sizeof(sourceNode));
	ifs.read((char *) &lastUsedMethod, sizeof(lastUsedMethod));
	ifs.read((char *) &sumPerformanceCounter, sizeof(sumPerformanceCounter));
	ifs.read((char *) &nSolutions, sizeof(nSolutions));
	parentNodeCosts.resize(nSolutions);
	ifs.read((char *) parentNodeCosts.data(), sizeof(nodeCost) * nSolutions);
	ifs.close();

	if(parentNodeCosts == nodeCosts)
	{
		return true;
	}
	else
	{
		return false;
	}
}
