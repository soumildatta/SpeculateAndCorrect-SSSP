// TEST FILE: Only used for testing

#include "tGraph.h"

int main(int argc, char *argv[])
{
    string filename = argv[1];

    // ! NOTES ON PARSE DIMACS
    // it parses each line properly 
    // It skips the lines not starting with c 
    // Has right number of nodes and edges

    tGraph graph;
    graph.parseDimacs(filename);

    cout << graph.nNodes << endl;
    cout << graph.getnEdges();

    return 0;
}