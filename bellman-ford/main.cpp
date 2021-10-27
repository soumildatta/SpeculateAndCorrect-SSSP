#include "bellmanFord.h"
#include "tGraph.h"
#include "tEdgeList.h"
#include "tTimer.h"

tGraph processGraph(path &filename);

int main(int argc, char *argv[])
{
	// Temporary cmd args parse
	// TODO: cmd getopt config file
	path filename { argv[1] };
	tGraph graph { processGraph(filename) };

	int iterations { atoi(argv[2]) };
	double totalTime { 0 };

    cout << "Performing Standard Bellman-Ford" << endl;

	for(auto i { 0 }; i < iterations; ++i)
	{
		vector<nodeCost> nodeCosts;
		auto time { bellmanFord(graph, 0u, nodeCosts) };
		cout << time << endl;
		totalTime += time;
	}

	cout << "\nTotal time for " << iterations << " iterations: " << totalTime << endl;
	cout << "Average time for each iteration: " << totalTime/iterations << endl;

    return 0;
}

tGraph processGraph(path &filename)
{
	cout << "Processing Graph" << endl;

	tEdgeList DIMACSEdgeList;
	DIMACSEdgeList.parseDimacs(filename.c_str());

	tGraph graph;
	graph.convertEdgeList(DIMACSEdgeList);

	return graph;
}
