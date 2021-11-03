//#include "bellmanFord.h"
#include "tGraph.h"
#include "tEdgeList.h"
#include "tTimer.h"

#include <pthread.h>
#include <thread>
#include <cstdio>
using std::printf;

using std::thread;

tGraph processGraph(path &filename);

void *Testing(void *threadid);

int main(int argc, char *argv[])
{
	// Temporary cmd args parse
	// TODO: cmd getopt config file
//	path filename { argv[1] };
//	tGraph graph { processGraph(filename) };

	auto maxThreads { thread::hardware_concurrency() };
	cout << "Max threads supported by this system: " << maxThreads << endl;

	// Testing
	pthread_t threads[maxThreads];

	for(int i { 0 }; i < maxThreads; ++i)
	{
		cout << "main() : creating thread, " << i << endl;
		int check { pthread_create(&threads[i], NULL, Testing, (void *)i) };
		if(check)
		{
			cout << "Error" << endl;
			exit(-1);
		}
	}
	pthread_exit(NULL);

    return 0;
}

void *Testing(void *threadid) {
   long tid;
   tid = (long)threadid;
   printf("\n%lu\n", tid);
   pthread_exit(NULL);
}

//tGraph processGraph(path &filename)
//{
//	cout << "Processing Graph" << endl;
//
//	tEdgeList DIMACSEdgeList;
//	DIMACSEdgeList.parseDimacs(filename.c_str());
//
//	tGraph graph;
//	graph.convertEdgeList(DIMACSEdgeList);
//
//	return graph;
//}
