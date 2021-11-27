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

#include <atomic>
using std::atomic_uint64_t;
using std::memory_order_release;
using std::memory_order_acquire;
using std::memory_order_acq_rel;
using std::memory_order_relaxed;

#include <algorithm>
using std::min;

#include <fstream>
using std::ofstream;

// Define max threads for device
#define maxHardwareThreads thread::hardware_concurrency()

tGraph processGraph(path &filename);

void specAndCorr(tData &data);

int main(int argc, char *argv[])
{
	// If numThreads argument is empty, program uses max threads supported by hardware
	if(argc < 6 || argc > 7)
	{
		cout << "Invalid arguments. Argument list expected: <filename> <verifyFilename> <outputFilename> <iterations> <sourceNode> <optional = numThreads>" << endl;
		exit(-1);
	}

	path filename { argv[1] };
	path verifyFilename { argv[2] };
	path outputFilename { argv[3] };
	int iterations { atoi(argv[4]) };
	uint64_t sourceNode { (uint64_t) atoi(argv[5]) };

	// Assign max number of threads to be used by program
	auto maxThreads { maxHardwareThreads };
	if(argc == 7)
	{
		maxThreads = atoi(argv[6]);
	}

	cout << "Max threads supported by this system: " << maxHardwareThreads << endl;
	cout << "Using " << maxThreads << " threads with " << iterations << " iterations." << endl;


	// Struct to be passed as argument list
	struct tData *data = (struct tData *)malloc(sizeof(struct tData));

	// Process graph
	tGraph graph { processGraph(filename) };

	ofstream outputFile;
	outputFile.open(outputFilename);
	// Output CSV Headers
	outputFile << "Num Threads, Speedup, Time\n";

	double baselineTime { 0 };
	for(auto threadCount { 1u }; threadCount <= maxThreads; ++threadCount)
	{
		cout << "Num threads: " << threadCount << endl;
		cout << "Solution: " << endl;

		double totalTime { 0 };

		for(auto iteration { 0u }; iteration < iterations; ++iteration)
		{
			// Initialize argument struct
			data->nodes = graph.nodes;
			data->edges = graph.edges;

			// Initial costs are set to infinity. Source node is set to 0
			const nodeCost initialCost(~0u, INT32_MAX);
			vector<nodeCost> solution(graph.nNodes, initialCost);
			solution[sourceNode] = nodeCost(sourceNode, 0);
			data->solution = solution;

			// Initialize speculation pool with source node is in the pool
			vector<uint64_t> speculationPool(graph.nNodes, TNA());
			speculationPool[0u] = sourceNode;
			data->speculationPool.pool = speculationPool;
			data->speculationPool.removeIndex = 0u;
			data->speculationPool.addIndex = 1u;
			data->speculationPool.bufferSize = graph.nNodes;

			// Initialize correction pool that is empty
			vector<uint64_t> correctionPool(graph.nNodes, TNA());
			data->correctionPool.pool = correctionPool;
			data->correctionPool.removeIndex = 0u;
			data->correctionPool.addIndex = 0u;
			data->correctionPool.bufferSize = graph.nNodes;

			// Source node
			data->source = sourceNode;

			data->nIncompleteTasks = 1u;
			toAtomic(&data->abortFlag)->store(false, memory_order_release);

			// Threads
			thread *threads[threadCount];

			tTimer timer;
			for(auto i { 0u }; i < threadCount; ++i)
			{
				// Creating threads
				threads[i] = new thread(specAndCorr, ref(*data));

				if(threads[i] == nullptr)
				{
					cout << "Error" << endl;
					exit(-1);
				}
			}

			for(auto i { 0u }; i < threadCount; ++i)
			{
				// Joining threads
				threads[i]->join();
			}
			totalTime += timer.getTime();

			if(readSolution(verifyFilename, data->solution))
			{
				cout << "o";
				cout.flush();
			}
			else
			{
				cout << "x";
				cout.flush();
			}
		}

		auto avgIterationTime { totalTime/iterations };

		cout << endl << "Avg time / iteration: " << avgIterationTime << endl;

		// Calculate speedup -- use 1 threads as baseline
		if(threadCount == 1u)
		{
			baselineTime = avgIterationTime;
			cout << "Speedup: 0" << endl;
			outputFile << threadCount << ", 0," << avgIterationTime << "\n";
		}
		else
		{
			auto speedup { baselineTime / avgIterationTime };
			cout << "Speedup: " << speedup << endl;
			outputFile << threadCount << ", " << speedup << ", " << avgIterationTime << "\n";
		}

		cout << endl;
	}

	outputFile.close();

	free(data);
	pthread_exit(NULL);
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

	   myTaskToken = toAtomic(&data.correctionPool.pool[corrRemoveSlot])->exchange(TNA(), memory_order_acq_rel);

	   if(myTaskToken != TNA())
	   {
		   corrThreadNeedsWork = true;
	   }
	   else
	   {
		   myTaskToken = toAtomic(&data.speculationPool.pool[specRemoveSlot])->exchange(TNA(), memory_order_acq_rel);

		   if(myTaskToken != TNA())
		   {
			   specThreadNeedsWork = true;
		   }
		   else
		   {
			   // No task available
			   if(maxHardwareThreads > thread::hardware_concurrency())
			   {
				   std::this_thread::yield();
			   }
			   continue;
		   }
	   }

	   // Task Prologue
	   proximalNodeIndex = myTaskToken;

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
				   oldPathCost = data.solution[distalNodeIndex];
			   } while (!data.solution[distalNodeIndex].toAtomic()->compare_exchange_strong(oldPathCost.toUint64(), min(proposedCost.toUint64(), data.solution[distalNodeIndex].toUint64()), memory_order_acq_rel, memory_order_relaxed));

			   // There are two cases on success - actual path cost change, or parent change.
			   if(oldPathCost.cost > proposedCost.cost)
			   {
				   // Better path found. Note the change and queue the descendants
				   if(oldPathCost.cost != INT32_MAX)
				   {
					   // Edge has been relaxed before, put distal node into correction pool
					   tIndex corrAddSlot { toAtomic(&data.correctionPool.addIndex)->fetch_add(1u, memory_order_acq_rel) % data.correctionPool.bufferSize };
					   auto value { toAtomic(&data.correctionPool.pool[corrAddSlot])->exchange(distalNodeIndex, memory_order_release) };
//					   toAtomic(&data.correctionPool.pool[corrAddSlot])->store(distalNodeIndex, memory_order_release);

					   // New task in pool, add one to incomplete tasks
					   toAtomic(&data.nIncompleteTasks)->fetch_add(1u, memory_order_release);

					   if(value != TNA())
					   {
						   toAtomic(&data.abortFlag)->store(true, memory_order_release);
						   return;
					   }
				   }
				   else
				   {
					   tIndex specAddSlot { toAtomic(&data.speculationPool.addIndex)->fetch_add(1u, memory_order_acq_rel) % data.speculationPool.bufferSize };
					   auto value { toAtomic(&data.speculationPool.pool[specAddSlot])->exchange(distalNodeIndex, memory_order_release) };
					   // toAtomic(&data.speculationPool.pool[specAddSlot])->store(distalNodeIndex, memory_order_release);

					   // New task in pool, add one to incomplete tasks
					   toAtomic(&data.nIncompleteTasks)->fetch_add(1u, memory_order_release);

					   if(value != TNA())
					   {
						   toAtomic(&data.abortFlag)->store(true, memory_order_release);
						   return;
					   }
				   }
			   }
		   }
		   else
		   {
			   // This case should never happen with generational ordering
			   throw("Cannot relax");
			   toAtomic(&data.abortFlag)->store(true, memory_order_release);
		   }
	   }

	   // Task complete, decrement from number of incomplete tasks
	   toAtomic(&data.nIncompleteTasks)->fetch_sub(1u, memory_order_release);
   }

   return;
}

tGraph processGraph(path &filename)
{
	cout << "Processing Graph" << endl << endl;

	tEdgeList DIMACSEdgeList;
	DIMACSEdgeList.importDIMACSEdgeList(filename.c_str());

	tGraph graph;
	graph.convertEdgeList(DIMACSEdgeList);

	return graph;
}
