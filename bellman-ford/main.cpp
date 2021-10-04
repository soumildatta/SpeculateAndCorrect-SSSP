#include "bellmanFord.h"
#include "tGraph.h"
#include "tEdgeList.h"

tGraph processGraph(const path &filename);

int main(int argc, char *argv[])
{
	// Temporary cmd args parse
	// TODO: cmd getopt config file
	path filename { argv[1] };
	tGraph graph { processGraph(filename) };

    bellmanFord();

    return 0;
}

tGraph processGraph(const path &filename)
{
	tEdgeList DIMACSEdgeList;
	DIMACSEdgeList.parseDimacs(filename.c_str());

	tGraph graph;
	graph.convertEdgeList(DIMACSEdgeList);

	return graph;
}
