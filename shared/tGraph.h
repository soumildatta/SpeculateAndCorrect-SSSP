#ifndef TGRAPH_H
#define TGRAPH_H

#include <vector>
using std::vector;

#include <iostream>
using std::cout;
using std::endl;

#include <cassert>

#include <stdlib.h>
#include <cstdint>

#include <atomic>
using std::atomic;

#include "tEdgeList.h"

struct nodeCost
{
	uint32_t proximalNodeIndex;
	uint32_t cost;

	nodeCost(void) : proximalNodeIndex(~0u), cost(INT32_MAX) { return; }
	nodeCost(uint32_t _proximalNodeIndex, int32_t _cost) : proximalNodeIndex(_proximalNodeIndex), cost(_cost) { return; }
	~nodeCost() { return; }

	inline atomic<uint64_t> *toAtomic(void) { return (atomic<uint64_t> *) &proximalNodeIndex; }
	inline uint64_t &toUint64(void) { return *((uint64_t *) &proximalNodeIndex); }
};

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

	#define tGraphMemberList nodes, edges, edgeList, nNodes, nEdges

    // Constructor
    inline tGraph(void) : nNodes(0u), nEdges(0u) { return; }

    inline void convertEdgeList(tEdgeList &edgeList)
    {
        nodes.clear();
        edges.clear();
        nodes.resize(edgeList.getnNodes(), tCSRNode());
        edges.reserve(edgeList.getnEdges());

//        edgeList.sort(tEdgeList::compareEntries);

        // At this point, edge list is sorted, and all proximal nodes will be grouped together

        tCSRNode node;
        tCSREdge edge;
        auto prevNode { ~0u };

        for(const auto &item : edgeList)
        {   
            // Check is proximal node is still the same from the last iteration
            if(item.proximalNodeIdx != prevNode)
            {
                // New node
                node.startEdgeIdx = edges.size();
                node.nEdges = 0u;
                nodes[item.proximalNodeIdx] = node;
                prevNode = item.proximalNodeIdx;
            }

            // New edge discovered
            ++nodes[item.proximalNodeIdx].nEdges;
            edge.distalNodeIdx = item.distalNodeIdx;
            edge.weight = item.weight;

            // Add edge to vector
            edges.emplace_back(edge);
        }

        // Check sizes between edgelist and CSR
        assert(edgeList.getnNodes() == nodes.size());
        assert(edgeList.getnEdges() == edges.size());

        nNodes = nodes.size();
        nEdges = edges.size();
    }
};

#endif
