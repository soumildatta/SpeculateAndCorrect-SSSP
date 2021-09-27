// TEST FILE: Only used for testing

#include "tGraph.h"
#include "tTimer.h"
#include "tEdgeList.h"

int main(int argc, char *argv[])
{
    string filename = argv[1];

    // ! NOTES ON PARSE DIMACS
    // it parses each line properly 
    // It skips the lines not starting with c 
    // Has right number of nodes and edges

    // tTimer time;

    tEdgeList edgeList;
    edgeList.parseDimacs(filename);

    // // cout << graph.nNodes << endl;
    // // cout << graph.getnEdges();

    // cout << time.getTime();

    tGraph graph;
    graph.convertEdgeList(edgeList);

    return 0;
}