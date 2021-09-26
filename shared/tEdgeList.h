#ifndef TEDGELIST_H
#define TEDGELIST_H

#include <iostream>
using std::cout;
using std::endl;

#include <sstream>
using std::istringstream;

#include <string>
using std::string;
using std::getline;
using std::literals::string_literals::operator""s;

#include <fstream>
using std::ifstream;

#include <unordered_map>
using std::unordered_map;

#include <list>
using std::list;

struct tEdgeListItem
{
    inline tEdgeListItem(void) : proximalNodeIdx(~0u), distalNodeIdx(~0u), weight(~0u) { return; }

    inline tEdgeListItem(uint32_t _proximalNodeIdx, uint32_t _distalNodeIdx, int32_t _weight) { return; }

    uint32_t proximalNodeIdx;
    uint32_t distalNodeIdx;
    int32_t weight;

    #define tEdgeListItemMemberList proximalNodeIdx, distalNodeIdx, weight
};

struct tEdgeList : public list<tEdgeListItem>
{
    // Keeps track of the number of nodes
    uint32_t nNodes;

    inline tEdgeList(void) : nNodes(0u)
    {
        clear();
        return;
    }

    inline virtual ~tEdgeList(void)
    {
        return;
    }

    // TODO: Figure out what this does
    inline uint32_t getNodeID(unordered_map<string, uint32_t> &nodeIdToIndex, const string &nodeID)
    {
        return nodeIdToIndex.count(nodeID) == 0u ? nodeIdToIndex[nodeID] = nodeIdToIndex.size() : nodeIdToIndex[nodeID];
    }

    inline void parseDimacs(const string &filename)
    {
        ifstream file(filename);
        if(file.peek() == ifstream::traits_type::eof())
        {
            // TODO: throw error here
            throw("File empty");
        }

        unordered_map<string, uint32_t> nodeIdToIndex;
        tEdgeListItem edge;
        uint32_t lineNum { 0u };

        for(string line; getline(file, line);)
        {
            ++lineNum;

            string command;
            string src;
            string dest;
            string weight;

            istringstream iss(line);
            iss >> command;
            // Only lines starting with the 'a' command need to be processed
            if(command != "a"s) continue;

            // We are on the correct line now, process graph
            iss >> src;
            iss >> dest;
            iss >> weight;

            // ! PRINTING OUT FOR DEBUGGING
            // cout << src << "," << dest << "," << weight << endl;

            edge.proximalNodeIdx = getNodeID(nodeIdToIndex, src);
            edge.distalNodeIdx = getNodeID(nodeIdToIndex, dest);
            // TODO: Make sure no empty weights
            edge.weight = stoi(weight);

            // Push to struct list
            emplace_back(edge);
        }

        nNodes = nodeIdToIndex.size();
        file.close();
    }

    inline uint32_t getnEdges() const
    {
        return size();
    }
};

inline bool compareEntries(const tEdgeListEntry &lEntry, const tEdgeListEntry &rEntry)
{
    return lEntry.proximalNodeIndex != rEntry.proximalNodeIndex ? lEntry.proximalNodeIndex < rEntry.proximalNodeIndex : lEntry.distalNodeIndex < rEntry.distalNodeIndex;
}

#endif