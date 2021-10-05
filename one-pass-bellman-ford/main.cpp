#include "optimizedBellmanFord.h"
#include "tGraph.h"
#include "tEdgeList.h"
#include "tTimer.h"

tGraph processGraph(path &filename);

int main(int argc, char *argv[])
{
	path file { argv[1] };

	tGraph graph { processGraph(file) };

    optimizedBellmanFord(graph, 0u);
    
    return 0;
}

tGraph processGraph(path &filename)
{
	tEdgeList inputGraph;
	inputGraph.parseDimacs(filename);

	tGraph graph;
	graph.convertEdgeList(inputGraph);

	return graph;
}
