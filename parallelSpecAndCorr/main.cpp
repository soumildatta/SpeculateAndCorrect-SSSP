//#include "bellmanFord.h"
#include "tGraph.h"
#include "tEdgeList.h"
#include "tTimer.h"
#include "tData.h"
#include "tPool.h"
#include "UtilityFunctions.h"

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

#include <atomic>
using std::atomic_uint64_t;
using std::memory_order_release;
using std::memory_order_acquire;
using std::memory_order_acq_rel;

#define makeAtomicUint64(var) (*(atomic_uint64_t *) &var)

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
//	int iterations { atoi(argv[3]) };
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
	vector<atomic_uint64_t> speculationPool;
	speculationPool.emplace_back(makeAtomicUint64(sourceNode));
	data->speculationPool.pool = speculationPool;
	data->speculationPool.removeIndex.store(0u, memory_order_release);
	data->speculationPool.addIndex.store(1u, memory_order_release);
	data->speculationPool.bufferSize = graph.nNodes;

//	cout << data->speculationPool.pool[0] << endl;
//
//	cout << HERE << endl;

	// Initialize correction pool that is empty
	//	vector<uint32_t> correctionPool(graph.nNodes);
	vector<atomic_uint64_t> correctionPool;
	data->correctionPool.pool = correctionPool;
	data->correctionPool.removeIndex.store(0u, memory_order_release);
	data->correctionPool.addIndex.store(0u, memory_order_release);
	data->correctionPool.bufferSize = graph.nNodes;

	// Source node
	data->source = sourceNode;

	data->nIncompleteTasks.store(1u, memory_order_release);
	data->abortFlag.store(false, memory_order_release);

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
   tIndex specRemoveIndex, corrRemoveIndex;
   tIndex specRemoveSlot, corrRemoveSlot;
   tIndex myTaskToken;
   tIndex proximalNodeIndex;
//   tIndex currentEdgeIndex;
//   uint32_t numChildrenRemaining;
   bool specThreadNeedsWork { true };
   bool corrThreadNeedsWork { true };
//   bool threadDoingWork { false };

//   uint32_t threadRelativeIndex;

   // Ensure we have a slot to monitor for data arrival from each pool
   if(specThreadNeedsWork)
   {
	   specRemoveIndex = data.speculationPool.removeIndex.fetch_add(1u, memory_order_acq_rel);
	   specRemoveSlot = specRemoveIndex % data.speculationPool.bufferSize;
	   specThreadNeedsWork = false;
   }

   if(corrThreadNeedsWork)
   {
	   corrRemoveIndex = data.correctionPool.removeIndex.fetch_add(1u, memory_order_acq_rel);
	   corrRemoveSlot = corrRemoveIndex % data.correctionPool.bufferSize;
	   corrThreadNeedsWork = false;
   }

   myTaskToken = data.correctionPool.pool[corrRemoveIndex].exchange(TNA(), memory_order_acq_rel);

   if(myTaskToken != TNA())
   {
	   myTaskToken = data.speculationPool.pool[specRemoveIndex].exchange(TNA(), memory_order_acq_rel);
   }

   if(myTaskToken != TNA())
   {
	   // Task Prologue
	   proximalNodeIndex = myTaskToken;
//	   numChildrenRemaining = data.nodes[proximalNodeIndex].nEdges;
//	   currentEdgeIndex = data.nodes[proximalNodeIndex].startEdgeIdx;

	   for(auto edgeIndex { 0u }; edgeIndex < data.nodes[proximalNodeIndex].nEdges; ++edgeIndex)
	   {
		   auto childIndex { data.nodes[proximalNodeIndex].startEdgeIdx + edgeIndex };

		   // Relaxations
		   nodeCost proposedCost(proximalNodeIndex, data.solution[proximalNodeIndex].cost + data.edges[data.nodes[proximalNodeIndex].startEdgeIdx].weight);

		   if(data.solution[proximalNodeIndex].cost != INT32_MAX)
		   {
			   nodeCost oldPathCost;
			   do
			   {
				   nodeCost proposedCost(proximalNodeIndex, data.solution[proximalNodeIndex].cost + data.edges[data.nodes[proximalNodeIndex].startEdgeIdx].weight);
				   oldPathCost = data.solution[proximalNodeIndex];
			   } while (oldPathCost != makeAtomicUint64(data.solution[proximalNodeIndex]).compare_exchange_strong(oldPathCost, proposedCost, memory_order_acquire, memory_order_release));

		   }
	   }
   }







   while(data.abortFlag.load(memory_order_acquire) && data.nIncompleteTasks.load(memory_order_acquire))
   {
//	   uint32_t proximalNodeIndex;
//
//
//	   const auto &proximalNode { data.nodes[proximalNodeIndex] };
//
//	   for(auto edgeIndex { 0u }; edgeIndex < proximalNode.nEdges; ++edgeIndex)
//	   {
//		   const auto &edge { data.edges[proximalNode.startEdgeIdx + edgeIndex] };
//		   const auto &distalNodeIndex { edge.distalNodeIdx };
//		   const auto &weight { edge.weight };
//
//		   nodeCost proposedParentNodeCost(proximalNodeIndex, data.solution[proximalNodeIndex].cost + weight);
//
//		   if(data.solution[proximalNodeIndex].cost != INT32_MAX)
//		   {
//
//		   }
//	   }
//
////	   if(data.correctionPool.pool[])
//
//	   // TODO: Check thread slot for task
//
//	   // TODO: Perform relaxations
//
//	   // TODO: Add to correction or speculation pool accordingly
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


//cout << data.speculationPool.pool[0] << endl;


//	   cout << item << endl;
//	   item++;
//	   if(item == 5) threadNeedsWork = false;

	   // TODO: Remove from pool
//	   threadRelativeIndex = data.threadTrackIndex;
//	   data.threadTrackIndex += 1u;

//	   // Lock is released when out of scope
//	   {
//		   const lock_guard<mutex> lock(g_mutex);
//		   if(data.correctionPool.pool.size())
//		   {
//			   proximalNodeIndex = data.correctionPool.pool.back();
//			   data.correctionPool.pool.pop_back();
//		   }
//		   else
//		   {
//			   proximalNodeIndex = data.speculationPool.pool.back();
//			   data.speculationPool.pool.pop_back();
//		   }
//	   }
