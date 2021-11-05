//#include "bellmanFord.h"
#include "tGraph.h"
#include "tEdgeList.h"
#include "tTimer.h"
#include "tData.h"
#include "tPool.h"

#include <pthread.h>
#include <thread>
using std::thread;

#include <cstdio>
using std::printf;

// Define max threads for device
#define maxThreads thread::hardware_concurrency()

tGraph processGraph(path &filename);

void *Testing(void *threadid);

int main(int argc, char *argv[])
{
	// TODO: add source node and num threads to argument list
	if(argc != 5)
	{
		cout << "Invalid arguments. Argument list expected: <filename> <verifyFilename> <iterations>" << endl;
		exit(-1);
	}

	path filename { argv[1] };
	path verifyFilename { argv[2] };
	int iterations { atoi(argv[3]) };
	int sourceNode { atoi(argv[4]) };

	cout << "Max threads supported by this system: " << maxThreads << endl;

	// Testing threads
	pthread_t threads[maxThreads];

	// Struct to be passed as argument list
	struct tData *data = (struct tData *)malloc(sizeof(struct tData));

	// Process graph
	tGraph graph { processGraph(filename) };

	// Initialize argument struct
	data->nodes = graph.nodes;
	data->edges = graph.edges;

	// Initial costs are set to infinity. Source node is set to 0
	const nodeCost initialCost(~0u, INT32_MAX);
	vector<nodeCost> solution(graph.nNodes, initialCost);
	solution[sourceNode] = nodeCost(sourceNode, 0);
	data->solution = solution;

	// Initialize speculation pool with source node is in the pool
	vector<uint32_t> speculationPool(graph.nNodes, ~0u);
	data->speculationPool.pool = speculationPool;
	data->speculationPool.removeIndex = 0u;
	data->speculationPool.addIndex = 1u;
	data->speculationPool.bufferSize = speculationPool.size();

	// Initialize correction pool that is empty
	vector<uint32_t> correctionPool(graph.nNodes, ~0u);
	data->correctionPool.pool = correctionPool;
	data->correctionPool.removeIndex = 0u;
	data->correctionPool.addIndex = 0u;
	data->correctionPool.bufferSize = correctionPool.size();

	// Source node
	data->source = sourceNode;

	for(auto i { 0u }; i < maxThreads; ++i)
	{
		cout << "creating thread " << i << endl;
		int check { pthread_create(&threads[i], NULL, Testing, (void *)data) };
		if(check)
		{
			cout << "Error" << endl;
			exit(-1);
		}
	}

	for(auto i { 0u }; i < maxThreads; ++i)
	{
		cout << "joining thread " << i << endl;
		pthread_join(threads[i], (void **)~0u);
	}

	pthread_exit(NULL);
	free(data);
    return 0;
}

void *Testing(void *data)
{
   cout << ((struct tData*)data)->source << endl;
   pthread_exit(NULL);
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
