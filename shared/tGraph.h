#ifndef TGRAPH_H
#define TGRAPH_H

#include <filesystem>
using std::filesystem::path;

#include <vector>
using std::vector;

#include <stdlib.h>
#include <cstdint>

#include "tEdgeList.h"
// CSR Representation
struct tCSRNode
{
    uint32_t startEdgeIdx;
    uint32_t nEdges;
    #define tCSRNodeMemberList startEdgeIdx, nEdges
};

struct tCSREdge
{
    size_t weight;
    uint32_t distalNodeIdx;
    #define tCSREdgeMemberList weight, distalNodeIdx
};

struct tGraph
{
    vector<tCSRNode> nodes;
    vector<tCSREdge> edges;
    vector<tEdgeListItem> edgeList;

    uint32_t nNodes;
    uint32_t nEdges;

    // TODO: Memberlist here

    // Constructor
    inline tGraph(void) : nNodes(0u), nEdges(0u) { return; }

    // inline double readEdgeList(const path &filename)
    // {

    // }
};

#endif