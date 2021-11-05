//#include "bellmanFord.h"
#include "tGraph.h"
#include "tEdgeList.h"
#include "tTimer.h"
#include "tData.h"

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
	if(argc != 4)
	{
		cout << "Invalid arguments. Argument list expected: <filename> <verifyFilename> <iterations>" << endl;
		exit(-1);
	}

	path filename { argv[1] };
	path verifyFilename { argv[2] };
	int iterations { atoi(argv[3]) };

	cout << "Max threads supported by this system: " << maxThreads << endl;

	// Testing threads
	pthread_t threads[maxThreads];

	// Struct to be passed as argument list
	struct tData *data = (struct tData *)malloc(sizeof(struct tData));


	// Process graph
	tGraph graph { processGraph(filename) };

	// Initialize struct
	data->nodes = graph.nodes;
	data->edges = graph.edges;

	const nodeCost initialNodeCost(~0u, INT32_MAX);
	vector<nodeCost> solution(graph.nNodes, initialNodeCost);
	data->solution = solution;

//	vector<uint32_t> speculationPool(graph.nNodes, ~0u);
//	data->speculationPool = speculationPool;
//
//	vector<uint32_t> correctionPool(graph.nNodes, ~0u);
//	data->correctionPool = correctionPool;

	data->source = 0u;


	// TODO: initialize data struct for arguments
//	for(auto i { 0u }; i < maxThreads; ++i)
//	{
//		data[i].nodes = graph.nodes;
//		data[i].edges = graph.edges;
//
//		const nodeCost initialCost(~0u, INT32_MAX);
//		vector<nodeCost> solution(graph.nNodes, initialCost);
//		data[i].solution = solution;
//
//		vector<uint32_t> speculationPool(graph.nEdges, ~0u);
//		data[i].speculationPool = speculationPool;
//
//		vector<uint32_t> correctionPool(graph.nEdges, ~0u);
//		data[i].correctionPool = correctionPool;
//
//		data[i].source = 0u;
//	}

	for(auto i { 0u }; i < maxThreads; ++i)
	{
		cout << "creating thread " << i << endl;
		int check { pthread_create(&threads[i], NULL, Testing, (void *)i) };
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

    return 0;
}

void *Testing(void *threadid) {
   long tid;
   tid = (long)threadid;
//   printf("Exiting thread %lu\n", tid);
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
