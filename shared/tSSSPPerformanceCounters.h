#ifndef TSSSPSTATS_H
#define TSSSPSTATS_H

#include "UtilityFunctions.h"

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

inline ostream& operator<<(ostream &os, const tSSSPPerformanceCounters &item)
{
    auto &[tSSSPPerformanceCountersMemberList] { item };
    os << dumpVar(nNodesVisited) << endl;
    os << dumpVar(nAlreadyOptimal) << endl;
    os << dumpVar(nBetterParentIndex) << endl;
    os << dumpVar(nCannotRelax) << endl;
    os << dumpVar(nRelaxationAttempts) << endl;
    os << dumpVar(nRelaxations) << endl;
    os << dumpVar(nCorrections) << endl;
    os << dumpVar(nSpeculations) << endl;
    os << dumpVar(nPasses) << endl;
    return os;
}

#endif
