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

#include <filesystem>
using std::filesystem::path;

#include <fstream>
using std::ifstream;

#include <unordered_map>
using std::unordered_map;

#include <list>
using std::list;

#include "UtilityFunctions.h"
//#include "tException.h"

struct tEdgeListItem
{
    inline tEdgeListItem(void) : proximalNodeIdx(~0u), distalNodeIdx(~0u), weight(~0u) { return; }

    inline tEdgeListItem(uint32_t _proximalNodeIdx, uint32_t _distalNodeIdx, int32_t _weight) : proximalNodeIdx(_proximalNodeIdx), distalNodeIdx(_distalNodeIdx), weight(_weight) { return; }

    uint32_t proximalNodeIdx;
    uint32_t distalNodeIdx;
    int32_t weight;

    #define tEdgeListItemMemberList proximalNodeIdx, distalNodeIdx, weight
};

struct tLineEntry
{
	string proximalNode;
	string distalNode;
	string weight;
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

    inline virtual ~tEdgeList(void) { return; }

    // TODO: Figure out what this does
    inline uint32_t getNodeID(unordered_map<string, uint32_t> &nodeIdToIndex, const string &nodeID)
    {
        return nodeIdToIndex.count(nodeID) == 0u ? nodeIdToIndex[nodeID] = nodeIdToIndex.size() : nodeIdToIndex[nodeID];
    }

    inline static bool compareEdgeListEntries(const tLineEntry &lEntry, const tLineEntry &rEntry)
	{
		if (lEntry.proximalNode != rEntry.proximalNode)
		{
			return lEntry.proximalNode < rEntry.proximalNode;
		}
		else
		{
			return lEntry.distalNode < rEntry.distalNode;
		}
	}

    inline static bool compareEntries(const tLineEntry &lEntry, const tLineEntry &rEntry)
	{
		return lEntry.proximalNode != rEntry.proximalNode ? lEntry.proximalNode < rEntry.proximalNode : lEntry.distalNode < rEntry.distalNode;
	}

    inline void importDIMACSEdgeList(const string &filename)
	{
		tLineEntry edgeString;

		list<tLineEntry> edgeStrings;
		edgeStrings.clear();

		list<string> nodeLabels;
		nodeLabels.clear();

		ifstream file(filename);

		if (file.peek() == ifstream::traits_type::eof())
		{
			throw("Unable to read file");
		}

		unordered_map<string, uint32_t> nodeIdToIndex;
		tEdgeListItem edge;
		auto lineNum { 0u };

		// Process each line in the file
		for (string line; getline(file, line);)
		{
			++lineNum;

			string command;
			string src;
			string dest;

			istringstream iss(line);
			iss >> command;
			// Only lines starting with the 'a' command need to be processed
			if(command != "a"s) continue;

			// capture all arc string.
			iss >> src;
			iss >> dest;
			iss >> edgeString.weight;

			edgeString.proximalNode = Format("%16s", src.c_str());
			edgeString.distalNode = Format("%16s", dest.c_str());

			// Store edge and node labels
			edgeStrings.emplace_back(edgeString);

			nodeLabels.emplace_back(edgeString.proximalNode);
			nodeLabels.emplace_back(edgeString.distalNode);
		}

		file.close();

		nodeLabels.sort();
		nodeLabels.unique();

		// Create node labels.
		nodeIdToIndex.clear();
		for (const auto &nodeID : nodeLabels)
		{
			nodeIdToIndex[nodeID] = nodeIdToIndex.size();
		}

		edgeStrings.sort(compareEdgeListEntries);

		for (const auto &edgeString : edgeStrings)
		{
			edge.proximalNodeIdx = getNodeID(nodeIdToIndex, edgeString.proximalNode);
			edge.distalNodeIdx = getNodeID(nodeIdToIndex, edgeString.distalNode);
			// TODO: Make sure no empty weights
			edge.weight = stoi(edgeString.weight);

			// Push to struct list
			emplace_back(edge);
		}

		nNodes = nodeIdToIndex.size();
	}

    inline uint32_t getnEdges() const { return size(); }

    inline uint32_t getnNodes() const { return nNodes; }
};



#endif
