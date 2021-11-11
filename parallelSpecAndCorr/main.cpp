//#include "bellmanFord.h"
#include "tGraph.h"
#include "tEdgeList.h"
#include "tTimer.h"
#include "tData.h"
#include "tPool.h"

#include <thread>
using std::thread;

#include <functional>
using std::ref;

#include <cstdio>
using std::printf;

// Define max threads for device
#define maxThreads thread::hardware_concurrency()

tGraph processGraph(path &filename);

void *specAndCorr(tData &data);

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

	// Threads
	thread *threads[maxThreads];

	for(auto i { 0u }; i < maxThreads; ++i)
	{
		cout << "creating thread " << i << endl;
		threads[i] = new thread(specAndCorr, ref(*data));
		if(threads[i] == nullptr)
		{
			cout << "Error" << endl;
			exit(-1);
		}
	}

	for(auto i { 0u }; i < maxThreads; ++i)
	{
		cout << "joining thread " << i << endl;
		threads[i]->join();
	}

	pthread_exit(NULL);
	free(data);
    return 0;
}

//void *specAndCorr(void *data)
void *specAndCorr(tData &data)
{
   cout << data.source << endl;

   // TODO: Remove from pool

   // TODO: Check thread slot for task

   // TODO: Perform relaxations

   pthread_exit(NULL);
}

tGraph processGraph(path &filename)
{
	cout << "Processing Graph" << endl;

	tEdgeList DIMACSEdgeList;
	DIMACSEdgeList.importDIMACSEdgeList(filename.c_str());

	cout << HERE << endl;

	tGraph graph;
	graph.convertEdgeList(DIMACSEdgeList);

	return graph;
}
