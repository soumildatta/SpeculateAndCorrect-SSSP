#include "bellmanFord.h"
#include "tGraph.h"
#include "tEdgeList.h"
#include "tTimer.h"
#include "validation.h"
#include "performanceMetrics.h"

tGraph processGraph(path &filename);

int main(int argc, char *argv[])
{
	// Temporary cmd args parse
	path filename { argv[1] };
	path verifyFile { argv[2] };
	int iterations { atoi(argv[3]) };

	tGraph graph { processGraph(filename) };
	performanceMetrics performance;

	double totalTime { 0 };

    cout << "Performing Standard Bellman-Ford" << endl;

	for(auto i { 0 }; i < iterations; ++i)
	{
		vector<nodeCost> nodeCosts;
		auto time { bellmanFord(graph, 0u, nodeCosts, performance) };

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

		totalTime += time;
	}

	cout << "\nTotal time for " << iterations << " iterations: " << totalTime << endl;
	cout << "Average time for each iteration: " << totalTime/iterations << endl;
	cout << "Performance Metrics: " << endl;
	performance.printMetrics();

    return 0;
}

tGraph processGraph(path &filename)
{
	cout << "Processing Graph" << endl;

	tEdgeList DIMACSEdgeList;
	DIMACSEdgeList.importDIMACSEdgeList(filename.c_str());

	tGraph graph;
	graph.convertEdgeList(DIMACSEdgeList);

	return graph;
}
