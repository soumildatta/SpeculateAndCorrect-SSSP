#include "validation.h"

#include <fstream>
using std::ifstream;

#include <iostream>
    using std::cout;
    using std::endl;

#include "tGraph.h"
#include "UtilityFunctions.h"
#include "tSSSPPerformanceCounters.h"

enum eSSSPSolutionType : uint8_t
{
    noSolution = 0u,
    eSpecAndCorr,
    eBellmanFord,
    henrys,
};

// Method to read and verify solution
bool readSolution(const path &filepath, vector<nodeCost> &nodeCosts)
{
	tSSSPPerformanceCounters sumPerformanceCounter;
	uint32_t sourceNode;
	eSSSPSolutionType lastUsedMethod;
	double solutionTime;
	size_t nSolutions;

	// ParentNodeCosts will house the solutions from the solution file
	vector<nodeCost> parentNodeCosts;

	ifstream ifs { filepath };

	if (!ifs)
	{
		cout << "Error reading file at " << filepath << endl;
		exit(0);
	}

	// Read file
	ifs.read((char *) &lastUsedMethod, sizeof(lastUsedMethod));
	ifs.read((char *) &solutionTime, sizeof(solutionTime));
	ifs.read((char *) &sourceNode, sizeof(sourceNode));
	ifs.read((char *) &lastUsedMethod, sizeof(lastUsedMethod));
	ifs.read((char *) &sumPerformanceCounter, sizeof(sumPerformanceCounter));
	ifs.read((char *) &nSolutions, sizeof(nSolutions));
	parentNodeCosts.resize(nSolutions);
	ifs.read((char *) parentNodeCosts.data(), sizeof(nodeCost) * nSolutions);
	ifs.close();

	// Compare the two vectors with operator defined in UtilityFunctions
	if(parentNodeCosts == nodeCosts)
	{
		return true;
	}
	else
	{
		return false;
	}
}
