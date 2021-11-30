#include "tGraph.h"
#include "tEdgeList.h"
#include "tTimer.h"
#include "validation.h"
#include "tSSSPPerformanceCounters.h"
#include "specAndCorr.h"

#include <fstream>
using std::ofstream;

tGraph processGraph(const path &filename);

int main(int argc, char *argv[])
{
	if(argc != 6)
	{
		cout << "Invalid arguments. Expected argument list: <filename> <verifyFilename> <outputFilename> <iterations> <sourceNode>" << endl;
		exit(1);
	}

	path filename { argv[1] };
	path verifyFile { argv[2] };
	path outputFilename { argv[3] };
	int iterations { atoi(argv[4]) };
	uint32_t sourceNode { (uint32_t) atoi(argv[5]) };

	tGraph graph { processGraph(filename) };
	tSSSPPerformanceCounters performance;

	double totalTime { 0 };

	ofstream outputFile;
	outputFile.open(outputFilename);
	// Output CSV Headers
	outputFile << "Iteration, Time\n";

	cout << "Performing Optimized Bellman Ford" << endl;

	for(auto i { 0 }; i < iterations; ++i)
	{
		performance.clear();

		vector<nodeCost> nodeCosts;
		auto time { specAndCorr(graph, sourceNode, nodeCosts, performance) };

		// Checking solution
		if(readSolution(verifyFile, nodeCosts))
		{
			cout << "o";
			cout.flush();
		}
		else
		{
			cout << "x";
			cout.flush();
		}

		outputFile << (i + 1) << ", " << time << "\n";

		totalTime += time;
	}

	cout << "\nTotal time for " << iterations << " iterations: " << totalTime << endl;
	cout << "Average time for each iteration: " << totalTime/iterations << endl << endl;
	cout << performance << endl;

	outputFile.close();
    return 0;
}

tGraph processGraph(const path &filename)
{
	cout << "Processing Graph" << endl;

	tEdgeList DIMACSEdgeList;
	DIMACSEdgeList.importDIMACSEdgeList(filename.c_str());

	tGraph graph;
	graph.convertEdgeList(DIMACSEdgeList);

	return graph;
}
