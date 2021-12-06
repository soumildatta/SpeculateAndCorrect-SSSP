#include "bellmanFord.h"
#include "tGraph.h"
#include "tEdgeList.h"
#include "tTimer.h"
#include "validation.h"
#include "tSSSPPerformanceCounters.h"

#include <fstream>
using std::ofstream;

tGraph processGraph(path &filename);

int main(int argc, char *argv[])
{
	// Command line argument checking
	if(argc != 6)
	{
		cout << "Invalid arguments. Expected argument list: <filename> <verifyFilename> <outputFilename> <iterations> <sourceNode>" << endl;
		exit(1);
	}

	// Parse command line arguments
	path filename { argv[1] };
	path verifyFile { argv[2] };
	path outputFilename { argv[3] };
	int iterations { atoi(argv[4]) };
	uint32_t sourceNode { (uint32_t) atoi(argv[5]) };

	// Process the graph into the ADT
	tGraph graph { processGraph(filename) };
	tSSSPPerformanceCounters performance;

	double totalTime { 0 };

	ofstream outputFile;
	outputFile.open(outputFilename);
	// Output CSV Headers
	outputFile << "Iteration, Time\n";

    cout << "Performing Standard Bellman-Ford with " << iterations << " iterations." << endl;

    // Perform the algorithm for specified number of iterations
	for(auto i { 0 }; i < iterations; ++i)
	{
		// Clear performance counters so that only the last iteration is measured
		performance.clear();

		// Solution vector
		vector<nodeCost> nodeCosts;

		// Run algorithm and store runtime
		auto time { bellmanFord(graph, sourceNode, nodeCosts, performance) };

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

		// Write the iteration number and time taken to output file
		outputFile << (i + 1) << ", " << time << "\n";
		totalTime += time;
	}

	// Output statistics
	cout << "\nTotal time for " << iterations << " iterations: " << totalTime << endl;
	cout << "Average time for each iteration: " << totalTime/iterations << endl << endl;
	cout << performance << endl;

	outputFile.close();
    return 0;
}

// Method to parse the raw DIMACS dataset file in to a graph ADT
tGraph processGraph(path &filename)
{
	cout << "Processing Graph" << endl;

	tEdgeList DIMACSEdgeList;
	DIMACSEdgeList.importDIMACSEdgeList(filename.c_str());

	tGraph graph;
	graph.convertEdgeList(DIMACSEdgeList);

	return graph;
}
