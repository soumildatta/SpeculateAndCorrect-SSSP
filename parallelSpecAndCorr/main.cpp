//#include "bellmanFord.h"
#include "tGraph.h"
#include "tEdgeList.h"
#include "tTimer.h"
#include "tData.h"
#include "tPool.h"
#include "UtilityFunctions.h"
#include "validation.h"

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
using std::memory_order_relaxed;

#define makeAtomicUint64(var) (*(atomic_uint64_t *) &var)

// Define max threads for device
#define maxThreads 1

tGraph processGraph(path &filename);

void specAndCorr(tData &data);

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
	uint64_t sourceNode { (uint64_t) atoi(argv[4]) };

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
	vector<uint64_t> speculationPool(graph.nNodes, TNA());
	speculationPool[0u] = (makeAtomicUint64(sourceNode));
	data->speculationPool.pool = speculationPool;
	data->speculationPool.removeIndex = 0u;
	data->speculationPool.addIndex = 1u;
	data->speculationPool.bufferSize = graph.nNodes;


	// Initialize correction pool that is empty
	//	vector<uint32_t> correctionPool(graph.nNodes);
	vector<uint64_t> correctionPool(graph.nNodes, TNA());
	data->correctionPool.pool = correctionPool;
	data->correctionPool.removeIndex = 0u;
	data->correctionPool.addIndex = 0u;
	data->correctionPool.bufferSize = graph.nNodes;

	// Source node
	data->source = sourceNode;

	data->nIncompleteTasks = 1u;
	toAtomic(&data->abortFlag)->store(false, memory_order_release);

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

//	for(auto &item : data->solution)
//	{
//		cout << item.proximalNodeIndex << " " << item.cost << endl;
//	}

	if(readSolution(verifyFilename, data->solution))
	{
		cout << "o";
	}
	else
	{
		cout << "x";
	}

	pthread_exit(NULL);

	free(data);
    return 0;
}

//void *specAndCorr(void *data)
void specAndCorr(tData &data)
{
   tIndex specRemoveIndex, corrRemoveIndex;
   tIndex specRemoveSlot, corrRemoveSlot;
   tIndex myTaskToken;
   tIndex proximalNodeIndex;
   bool specThreadNeedsWork { true };
   bool corrThreadNeedsWork { true };

   while(!toAtomic(&data.abortFlag)->load(memory_order_acquire) && toAtomic(&data.nIncompleteTasks)->load(memory_order_acquire))
   {
//	   cout << data.nIncompleteTasks << endl;

	   // Ensure we have a slot to monitor for data arrival from each pool
	   if(specThreadNeedsWork)
	   {
		   specRemoveIndex = toAtomic(&data.speculationPool.removeIndex)->fetch_add(1u, memory_order_acq_rel);
		   specRemoveSlot = specRemoveIndex % data.speculationPool.bufferSize;
		   specThreadNeedsWork = false;
	   }

	   if(corrThreadNeedsWork)
	   {
		   corrRemoveIndex = toAtomic(&data.correctionPool.removeIndex)->fetch_add(1u, memory_order_acq_rel);
		   corrRemoveSlot = corrRemoveIndex % data.correctionPool.bufferSize;
		   corrThreadNeedsWork = false;
	   }

	   // TODO: Added if statement around correction remove
	   if(data.correctionPool.pool[corrRemoveSlot] != TNA())
	   {
		   myTaskToken = toAtomic(&data.correctionPool.pool[corrRemoveSlot])->exchange(TNA(), memory_order_acq_rel);
	   }

	   if(myTaskToken != TNA())
	   {
		   myTaskToken = toAtomic(&data.speculationPool.pool[specRemoveSlot])->exchange(TNA(), memory_order_acq_rel);
	   }


	   if(myTaskToken != TNA())
	   {
		   // TODO: WORKS HERE
		   // libc++abi: terminating with uncaught exception of type char const*

		   // Task Prologue
		   proximalNodeIndex = myTaskToken;

		   // TODO: CHeck for negative cycles

		   for(auto edgeIndex { 0u }; edgeIndex < data.nodes[proximalNodeIndex].nEdges; ++edgeIndex)
		   {
			   auto currentEdgeIndex { data.nodes[proximalNodeIndex].startEdgeIdx + edgeIndex };
			   auto distalNodeIndex { data.edges[currentEdgeIndex].distalNodeIdx };

			   // Relaxations
			   nodeCost proposedCost(proximalNodeIndex, data.solution[proximalNodeIndex].cost + data.edges[currentEdgeIndex].weight);

			   if(data.solution[proximalNodeIndex].cost != INT32_MAX)
			   {
				   nodeCost oldPathCost;
				   do
				   {
					   oldPathCost = data.solution[proximalNodeIndex];
				   } while (!data.solution[proximalNodeIndex].toAtomic()->compare_exchange_strong(oldPathCost.toUint64(), proposedCost.toUint64(), memory_order_acq_rel, memory_order_relaxed));

//				   if(oldPathCost.cost > proposedCost.cost)
//				   {
//					   cout << "Hello" << endl;
//					   data.solution[distalNodeIndex].toAtomic()->store(proposedCost.toUint64(), memory_order_release);
//				   }
//				   else
//				   {
//					   cout << "Hello 2" << endl;
//					   data.solution[distalNodeIndex].toAtomic()->store(proposedCost.toUint64(), memory_order_release);
//				   }
//break;
				   // There are two cases on success - actual path cost change, or parent change.
				   if(oldPathCost.cost > proposedCost.cost)
				   {
					   // Better path found. Note the change and queue the descendants
					   data.solution[distalNodeIndex].toAtomic()->store(proposedCost.toUint64(), memory_order_release);

					   if(oldPathCost.cost != INT32_MAX)
					   {
						   // Edge has been relaxed before, put distal node into correction pool
						   tIndex corrAddSlot { toAtomic(&data.correctionPool.addIndex)->fetch_add(1u, memory_order_acq_rel) % data.correctionPool.bufferSize };
						   toAtomic(&data.correctionPool.pool[corrAddSlot])->store(distalNodeIndex, memory_order_release);
					   }
					   else
					   {
						   tIndex specAddSlot { toAtomic(&data.speculationPool.addIndex)->fetch_add(1u, memory_order_acq_rel) % data.speculationPool.bufferSize };
						   auto value { toAtomic(&data.speculationPool.pool[specAddSlot])->exchange(distalNodeIndex, memory_order_release) };
						   if(value != TNA())
						   {
							   toAtomic(&data.abortFlag)->store(true, memory_order_release);
							   throw("Error Correction");
							   return;
						   }
					   }
				   }
				   else
				   {
					   data.solution[distalNodeIndex].toAtomic()->store(proposedCost.toUint64(), memory_order_release);
					   auto value { data.speculationPool.pool.emplace_back(distalNodeIndex) };
					   if(value != TNA())
					   {
						   toAtomic(&data.abortFlag)->store(true, memory_order_release);
						   throw("Error Speculation");
						   return;
					   }
				   }
			   }
			   else
			   {
				   // This case should never happen for generational ordering
				   throw("Cannot relax");
				   toAtomic(&data.abortFlag)->store(true, memory_order_release);
			   }
		   }

		   printf("sub");
		   toAtomic(&data.nIncompleteTasks)->fetch_sub(1u, memory_order_release);

	   }
   }

   cout << "Thread has finished" << endl;
   return;
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
