#include "optimizedBellmanFord.h"
#include "tGraph.h"
#include "tEdgeList.h"
#include "tTimer.h"
#include "validation.h"

tGraph processGraph(const path &filename);

int main(int argc, char *argv[])
{
	path filename { argv[1] };
	path verifyFile { argv[2] };
	tGraph graph { processGraph(filename) };

	int iterations { atoi(argv[3]) };
	double totalTime { 0 };

	cout << "Performing Optimized Bellman Ford" << endl;

	for(auto i { 0 }; i < iterations; ++i)
	{
		vector<nodeCost> nodeCosts;
		auto time { optimizedBellmanFord(graph, 0u, nodeCosts) };

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
