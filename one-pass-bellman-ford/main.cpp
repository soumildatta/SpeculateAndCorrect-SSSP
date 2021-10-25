#include "optimizedBellmanFord.h"
#include "tGraph.h"
#include "tEdgeList.h"
#include "tTimer.h"

tGraph processGraph(const path &filename);

int main(int argc, char *argv[])
{
	path filename { argv[1] };
	tGraph graph { processGraph(filename) };

	int iterations { atoi(argv[2]) };
	double totalTime { 0 };

	for(auto i { 0 }; i < iterations; ++i)
	{
		auto time { optimizedBellmanFord(graph, 0u) };
		cout << time << endl;
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
	DIMACSEdgeList.parseDimacs(filename.c_str());

	tGraph graph;
	graph.convertEdgeList(DIMACSEdgeList);

	return graph;
}
