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

#include <list>
using std::list;

#include <mutex>
using std::mutex;
using std::lock_guard;

// Define max threads for device
#define maxThreads thread::hardware_concurrency()

tGraph processGraph(path &filename);

void *specAndCorr(tData &data);

mutex g_mutex;

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
	//	vector<uint32_t> speculationPool(graph.nNodes);
	vector<uint32_t> speculationPool;
	speculationPool.emplace_back(sourceNode);
	data->speculationPool.pool = speculationPool;
	data->speculationPool.removeIndex = 0u;
	data->speculationPool.addIndex = 1u;
	data->speculationPool.bufferSize = graph.nNodes;

//	cout << data->speculationPool.pool[0] << endl;
//
//	cout << HERE << endl;

	// Initialize correction pool that is empty
	//	vector<uint32_t> correctionPool(graph.nNodes);
	vector<uint32_t> correctionPool;
	data->correctionPool.pool = correctionPool;
	data->correctionPool.removeIndex = 0u;
	data->correctionPool.addIndex = 0u;
	data->correctionPool.bufferSize = graph.nNodes;

	// Source node
	data->source = sourceNode;

	// Updates an index for each thread
	data->threadTrackIndex = 0u;

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
   bool threadNeedsWork { true };

   uint32_t threadRelativeIndex;

   cout << data.speculationPool.pool[0] << endl;

   int item = 0;
   while(threadNeedsWork && (data.speculationPool.pool.size() || data.correctionPool.pool.size()))
   {
	   uint32_t proximalNodeIndex;

	   cout << item << endl;
	   item++;
	   if(item == 5) threadNeedsWork = false;

	   // TODO: Remove from pool
//	   threadRelativeIndex = data.threadTrackIndex;
//	   data.threadTrackIndex += 1u;

	   // Lock is released when out of scope
	   {
		   const lock_guard<mutex> lock(g_mutex);
		   if(data.correctionPool.pool.size())
		   {
			   proximalNodeIndex = data.correctionPool.pool.back();
			   data.correctionPool.pool.pop_back();
		   }
		   else
		   {
			   proximalNodeIndex = data.speculationPool.pool.back();
			   data.speculationPool.pool.pop_back();
		   }
	   }

	   const auto &proximalNode { data.nodes[proximalNodeIndex] };

	   for(auto edgeIndex { 0u }; edgeIndex < proximalNode.nEdges; ++edgeIndex)
	   {
		   const auto &edge { data.edges[proximalNode.startEdgeIdx + edgeIndex] };
		   const auto &distalNodeIndex { edge.distalNodeIdx };
		   const auto &weight { edge.weight };

		   nodeCost proposedParentNodeCost(proximalNodeIndex, data.solution[proximalNodeIndex].cost + weight);

		   if(data.solution[proximalNodeIndex].cost != INT32_MAX)
		   {

		   }
	   }

//	   if(data.correctionPool.pool[])

	   // TODO: Check thread slot for task

	   // TODO: Perform relaxations

	   // TODO: Add to correction or speculation pool accordingly
   }

   cout << "Thread has finished" << endl;
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
