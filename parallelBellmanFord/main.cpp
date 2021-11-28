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

void bellmanFord(tData &data);

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
			vector<uint64_t> pool(graph.nNodes, TNA());
			pool[0u] = sourceNode;
			data->pool.pool = pool;
			data->pool.removeIndex = 0u;
			data->pool.addIndex = 1u;
			data->pool.bufferSize = graph.nNodes;

			// Source node
			data->source = sourceNode;

			data->nPasses = 0u;
			data->nIncompleteTasks = 1u;
			data->nNodes = graph.nNodes;
			toAtomic(&data->abortFlag)->store(false, memory_order_release);

			// Threads
			thread *threads[threadCount];

			tTimer timer;
			for(auto i { 0u }; i < threadCount; ++i)
			{
				// Creating threads
				threads[i] = new thread(bellmanFord, ref(*data));

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
void bellmanFord(tData &data)
{
   tIndex poolRemoveIndex, poolRemoveSlot;

   tIndex myTaskToken;
   tIndex proximalNodeIndex;
   bool threadNeedsWork { true };


   while(!toAtomic(&data.abortFlag)->load(memory_order_acquire) && !(toAtomic(&data.nPasses)->load(memory_order_acquire) == (data.nNodes * data.nNodes)-data.nNodes))
   {
	   // Ensure we have a slot to monitor for data arrival from each pool
	   if(threadNeedsWork)
	   {
		   poolRemoveIndex = toAtomic(&data.pool.removeIndex)->fetch_add(1u, memory_order_acq_rel);
		   poolRemoveSlot = poolRemoveIndex % data.pool.bufferSize;
		   threadNeedsWork = false;
	   }

	   myTaskToken = toAtomic(&data.pool.pool[poolRemoveSlot])->exchange(TNA(), memory_order_acq_rel);

	   if(myTaskToken != TNA())
	   {
		   threadNeedsWork = true;
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

	   // Task Prologue
	   proximalNodeIndex = myTaskToken;

	   for(auto edgeIndex { 0u }; edgeIndex < data.nodes[proximalNodeIndex].nEdges; ++edgeIndex)
	   {
		   auto currentEdgeIndex { data.nodes[proximalNodeIndex].startEdgeIdx + edgeIndex };
		   auto distalNodeIndex { data.edges[currentEdgeIndex].distalNodeIdx };

		   cout << dumpVar(proximalNodeIndex) << endl;

		   nodeCost proposedCost(proximalNodeIndex, toAtomic(&data.solution[proximalNodeIndex].cost)->load(memory_order_acquire) + data.edges[currentEdgeIndex].weight);

		   if(toAtomic(&data.solution[proximalNodeIndex].cost)->load(memory_order_acquire) != INT32_MAX)
		   {
			   if(proposedCost.cost < data.solution[distalNodeIndex].cost)
			   {
				   // Better path found
				   toAtomic(&data.solution[distalNodeIndex].toUint64())->store(proposedCost.toUint64(), memory_order_release);
			   }
		   }
		   else
		   {
			   if(proposedCost.cost == data.solution[distalNodeIndex].cost && (proposedCost.proximalNodeIndex < data.solution[distalNodeIndex].proximalNodeIndex))
			   {
				   toAtomic(&data.solution[distalNodeIndex].proximalNodeIndex)->store(proximalNodeIndex, memory_order_release);
			   }
		   }

		   // Add back to pool
		   tIndex poolAddSlot { toAtomic(&data.pool.addIndex)->fetch_add(1u, memory_order_acq_rel) % data.pool.bufferSize };
		   auto value { toAtomic(&data.pool.pool[poolAddSlot])->exchange(distalNodeIndex, memory_order_release) };
//		   toAtomic(&data.nIncompleteTasks)->fetch_add(1u, memory_order_release);
	   }

	   toAtomic(&data.nPasses)->fetch_add(1u, memory_order_release);

	   // Task complete, decrement from number of incomplete tasks
//	   toAtomic(&data.nIncompleteTasks)->fetch_sub(1u, memory_order_release);
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
